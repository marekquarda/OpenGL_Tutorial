#include <iostream>
#include <stdint.h> 
#include <vector>
#include "stbi_image_write.h"
#include <glm/glm.hpp>


struct Frame{
    std::vector<uint8_t>data;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t channels = 0;
    Frame(){};
    Frame(uint32_t w, uint32_t h, uint32_t c): width(w), height(h), channels(c),data(w*h*c){};
};

void flipFrame(Frame&frame) {
    for (uint32_t y = 0; y< frame.height/2;++y) {
        for (uint32_t x = 0; x< frame.width;++x) {
            for (uint32_t c = 0; c< frame.channels;++c) {
                auto src = (y*frame.width+x)*frame.channels+c;
                auto dst = ((frame.height-1-y)*frame.width+x)*frame.channels+c;
                auto z = frame.data.at(src);
                frame.data.at(src) = frame.data.at(dst);
                frame.data.at(dst) = z;
            }
        }
    }
}

void saveFrame(std::string const&file, Frame const&f) {
    auto frame = f;
    flipFrame(frame);
    stbi_write_png(file.c_str(), frame.width,frame.height, 3, frame.data.data(),0);
    std::cerr << "storing screenshot to: \"" << file << "\"" << std::endl;
}

struct Sphere{
    glm::vec3   C;
    float       R;
    glm::vec3   color;
    float       mirror;
    Sphere(){};
    Sphere(glm::vec3 const&c, float r, glm::vec3 const&col, float m):C(c),R(r),color(col),mirror(m){};
};

struct Ray
{
    glm::vec3 S;
    glm::vec3 D;
};

struct Intersection 
{
    glm::vec3 position = glm::vec3(0,0,0);
    glm::vec3 normal =  glm::vec3(1,0,0);
    float t = -1.f;
    size_t material = 0;
};

Intersection raySphereIntersection(Sphere const&s, Ray const&ray) {
    Intersection inter;
    float a = glm::dot(ray.D,ray.D);
    float b = 2*glm::dot(ray.D, ray.S-s.C);
    float c = glm::dot(ray.S-s.C,ray.S-s.C)-s.R*s.R;
    float d = b*b-4*a*c;
    if(d<0.f) return inter;
    float t1 = (-b-glm::sqrt(d))/(2*a);
    float t2 = (-b+glm::sqrt(d))/(2*a);
    float t;
    if (t1<0.f) t= t2;
    else if (t2<0.f) t= t1;
    else t = glm::min(t1,t2); 
    inter.t = t;
    inter.position = ray.S + ray.D * t;
    inter.normal = glm::normalize(inter.position-s.C);
    return inter;
}

float uniform() {
    return (float) rand() / (float) RAND_MAX;
}

float random(float mmin, float mmax) {
    return mmin + uniform()*(mmax-mmin);;
}

glm::vec3 random(glm::vec3 const&mmin, glm::vec3 const&mmax) {
    glm::vec3 res;
    for (int i = 0; i < 3; ++i) 
        res[i] = random(mmin[i], mmax[i]);
    return res;        
}

Intersection findIntersection(std::vector<Sphere>const& spheres, Ray const&ray) {
    Intersection finalInter;
    for (size_t i = 0; i < spheres.size(); ++i) {
        auto const&sphere = spheres.at(i);
        auto inter = raySphereIntersection(sphere, ray);
        if(inter.t>0.f) {
            if(finalInter.t<0.f || inter.t < finalInter.t) {
                finalInter = inter;
                finalInter.material = i;
            }
        }
    }
    return finalInter;
}

glm::vec3 computeLambert(std::vector<Sphere> const& spheres, Intersection const&inter, glm::vec3 const&lightPosition) {
    if (inter.t< 0.f) {
        return glm::vec3(0.1f);
    }
    auto const&sphere = spheres.at(inter.material);
    auto const&diffuseColor = sphere.color;
    auto const&N = inter.normal;
    auto L = glm::normalize(lightPosition-inter.position);
    float Df = glm::max(glm::dot(N, L),0.f);

    Ray shadowRay;
    shadowRay.D = L;
    shadowRay.S = inter.position + shadowRay.D*0.001f;
    
    auto shadowInter = findIntersection(spheres, shadowRay);
    if (shadowInter.t>.0f)
        Df= 0.f;

    float Af = 0.1f;
    auto finalColor = diffuseColor*Df + diffuseColor*Af;
    return finalColor;
}

int main(int args, char*argv[]) {
    srand(time(0));
    Frame frame = Frame(1024,768,3);

    std::vector<Sphere> spheres;
    spheres.emplace_back(glm::vec3(+1,0,-3),1.f,glm::vec3(0,0,1),0.75);
    spheres.emplace_back(glm::vec3(-1,0,-4),1.f,glm::vec3(0.9),0.75);

    uint32_t N=20;
    for (uint32_t i=0;i<N;++i) {
        spheres.emplace_back(
            random(glm::vec3(-3),glm::vec3(-10)),
            random(.1f,5.f),
            random(glm::vec3(0), glm::vec3(1)), 
            random(0.f,1.f)
        );
    }

    auto lightPosition = glm::vec3(100,100,100);
     
    float aspect = (float) frame.width/(float) frame.height;
    for (uint32_t y=0;y<frame.height;++y) {
        for(u_int32_t x=0;x<frame.width;++x) {
            float fx = (float)x/(float)(frame.width-1)*2.f-1.f;
            float fy = (float)y/(float)(frame.height-1)*2.f-1.f;
            fx*=aspect;
            Ray ray;
            ray.S = glm::vec3(0.f);
            ray.D = glm::normalize(glm::vec3(fx, fy, -1.f));

            auto primaryInter = findIntersection(spheres, ray);
            auto finalColor = computeLambert(spheres, primaryInter, lightPosition);

            float primaryMirror = spheres.at(primaryInter.material).mirror;
            finalColor *= (1.f-primaryMirror);
 
            float weight = primaryMirror;
            Ray refRay = ray; 
            Intersection inter = primaryInter;

            for(uint32_t d=0;d<4;++d) {
                if (weight > 0.f && inter.t > 0.f) {
                    refRay.D = glm::reflect(refRay.D,inter.normal);
                    refRay.S = inter.position + refRay.D*0.0001f;
                    inter = findIntersection(spheres, refRay);
                    auto refColor =  computeLambert(spheres, inter, lightPosition);
                    auto mirror = spheres.at(inter.material).mirror;
                    finalColor += refColor*weight*(1-mirror);
                    weight*=mirror;
                }    
            }

            for (uint32_t c=0; c < frame.channels;++c) 
                frame.data[(y*frame.width+x)*frame.channels+c] = (uint32_t) glm::clamp(finalColor[c]*255.f,0.f,255.f);


              
            // if(finalInter.t>0.f) {
            //     auto finalColor = computeLambert(spheres, finalInter, lightPosition);
            //     // reflect (Phong)
            //     // glm::vec3 reflectSource = glm::normalize(glm::reflect(lightPosition, N));
            //     // float specularStrength = glm::max(glm::dot(ray.D, reflectSource), 0.f);
            //     // specularStrength = glm::pow(specularStrength, 32.f);
            //     // glm::vec3 light = glm::vec3(1.f,1.f,1.f);
            //     // glm::vec3 specular = specularStrength * light;
            //     // end relect
            //     auto const&sphere = spheres.at(finalInter.material);
            //     finalColor *= (1.f-sphere.mirror);
 
            //     if (sphere.mirror > 0.f) {
            //         Ray refRay;
            //         refRay.D = glm::reflect(ray.D,finalInter.normal);
            //         refRay.S = finalInter.position;
            //         auto refInter = findIntersection(spheres, refRay);
            //     }

            //     for (uint32_t c=0; c < frame.channels;++c) 
            //     frame.data[(y*frame.width+x)*frame.channels+c] = (uint32_t) glm::clamp(finalColor[c]*255.f,0.f,255.f);
            // } else {
            //     for (uint32_t c=0; c < frame.channels;++c) 
            //         frame.data[(y*frame.width+x)*frame.channels+c] = 40;
            // }
        }
     }

    saveFrame("output.png", frame);
    return 1;
}