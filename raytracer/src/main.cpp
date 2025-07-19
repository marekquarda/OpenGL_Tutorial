#include <iostream>
#include <vector>
#include "stbi_image_write.h"



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

int main(int args, char*argv[]) {
    Frame frame = Frame(256,256,3);
    saveFrame("output.png", frame);
    return 1;
}