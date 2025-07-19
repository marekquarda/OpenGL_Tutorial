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
    glm::vec3 C;
    float     R;
};

struct Ray
{
    glm::vec3 S;
    glm::vec3 D;
};


float raySphereIntersection(Sphere const&s, Ray const&ray) {
    float a = glm::dot(ray.D,ray.D);
    float b = 2*glm::dot(ray.D, ray.S-s.C);
    float c = glm::dot(ray.S-s.C,ray.S-s.C)-s.R*s.R;
    float d = b*b-4*a*c;
    if(d<0.f) return -1.f;
    float t1 = (-b-glm::sqrt(d))/(2*a);
    float t2 = (+b-glm::sqrt(d))/(2*a);
    if (t1<0.f) return t2;
    if (t2<0.f) return t1;
    return glm::min(t1,t2);
}

int main(int args, char*argv[]) {
    Frame frame = Frame(1024,1024,3);
    for (uint32_t y=0;y<frame.width;++y) {
        for(u_int32_t x=0;x<frame.height;++x) {
            float fx = (float)x/(float)(frame.width-1)*2.f-1.f;
            float fy = (float)y/(float)(frame.height-1)*2.f-1.f;
            auto S = glm::vec3(0.f);
            auto D = glm::normalize(glm::vec3(fx, fy, -1.f));

            frame.data[(y*frame.width+x)*frame.channels+0] = (uint8_t) glm::clamp(255.f*(D.x*.5f+.5f),0.f,255.f);
            frame.data[(y*frame.width+x)*frame.channels+1] = (uint8_t) glm::clamp(255.f*(D.y*.5f+.5f),0.f,255.f);
            
        }
    }

    saveFrame("output.png", frame);
    return 1;
}