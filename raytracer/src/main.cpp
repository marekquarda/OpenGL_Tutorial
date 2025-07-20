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
        for (uint32_t x = 0; x< frame.width/2;++x) {
            for (uint32_t c = 0; c< frame.channels/2;++c) {
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
    glm::vec3 color;
    glm::vec3 C;
    float     R;
    Sphere(){};
    Sphere(glm::vec3 const&c, float r, glm::vec3 const&col):C(c),R(r),color(col){};
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

int main(int args, char*argv[]) {
    Frame frame = Frame(1024,1024,3);

    std::vector<Sphere> spheres;
    spheres.emplace_back(glm::vec3(0,0,-3),1.f,glm::vec3(0,0,1));
    spheres.emplace_back(glm::vec3(-1,0,-4),1.f,glm::vec3(0,1,0));
     
    float aspect = (float) frame.width/(float) frame.height;
    for (uint32_t y=0;y<frame.height;++y) {
        for(u_int32_t x=0;x<frame.width;++x) {
            float fx = (float)x/(float)(frame.width-1)*2.f-1.f;
            float fy = (float)y/(float)(frame.height-1)*2.f-1.f;
            fx*=aspect;
            Ray ray;
            ray.S = glm::vec3(0.f);
            ray.D = glm::normalize(glm::vec3(fx, fy, -1.f));

            float minT = -1.f;
            size_t sphereId = 0;
            Intersection finalinter;
            for (size_t i = 0; i < spheres.size(); ++i) {
                auto const&sphere = spheres.at(i);
                auto inter = raySphereIntersection(sphere, ray);
                if(inter.t>0.f) {
                    if(minT<0.f || inter.t < minT) {
                        minT = inter.t;
                        sphereId = i;
                        finalinter = inter;
                    }
                }
            }
              
            if(minT>0.f) {
                auto const&sphere = spheres.at(sphereId);
                auto const&color = sphere.color;
                auto const&normal = finalinter.normal;
                for (uint32_t c=0; c < frame.channels;++c) 
                frame.data[(y*frame.width+x)*frame.channels+c] = (uint32_t) glm::clamp(normal[c]*255.f,0.f,255.f);
            } else {
                for (uint32_t c=0; c < frame.channels;++c) 
                    frame.data[(y*frame.width+x)*frame.channels+c] = 40;
            }
        }
            // frame.data[(y*frame.width+x)*frame.channels+0] = (uint8_t) glm::clamp(255.f*(D.x*.5f+.5f),0.f,255.f);
           // frame.data[(y*frame.width+x)*frame.channels+1] = (uint8_t) glm::clamp(255.f*(D.y*.5f+.5f),0.f,255.f);
    }

    saveFrame("output.png", frame);
    return 1;
}