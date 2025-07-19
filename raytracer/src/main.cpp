#include "stb_image_write.h"
#include <vector>

#include <iostream>

struct Frame{
    std::vector<uint8_t>data;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t channels = 0;
};

void flipFrame(Frame&frame) {
    for (uint32_t y = 0; y< frame.height/2;++y) {
        for (uint32_t x = 0; x< frame.width/2;++x) {
            for (uint32_t c = 0; c< frame.channels/2;++c) {
                auto src = (y*frame.width+x)*frame.channels+c;
                auto dst = ((y*frame.height-1-y)*frame.width+x)*frame.channels+c;
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
    return 1;
}