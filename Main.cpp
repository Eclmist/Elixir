#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stbi_image_write.h"

#define OUTPUT_WIDTH 800
#define OUTPUT_HEIGHT 460
#define NUM_CHANNELS 3

int main()
{
    // PPM Headers
    std::string header = "P6\n" + std::to_string(OUTPUT_WIDTH) + " " + std::to_string(OUTPUT_HEIGHT) + "\n255\n";
    
    std::vector<unsigned char> buffer(header.begin(), header.end());

    for (int y = 0; y < OUTPUT_HEIGHT; y++)
    {
        for (int x = 0; x < OUTPUT_WIDTH; x++)
        {
            unsigned char r = static_cast<unsigned char>(x / 800.0f * 255);
            unsigned char g = static_cast<unsigned char>(y / 460.0f * 255);
            unsigned char b = static_cast<unsigned char>(100);

            buffer.push_back(r);
            buffer.push_back(g);
            buffer.push_back(b);
        }
    }

    int x, y, n;
    stbi_uc* output = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size()), &x, &y, &n, 0);
    stbi_write_png("output.png", OUTPUT_WIDTH, OUTPUT_HEIGHT, NUM_CHANNELS, output, OUTPUT_WIDTH * NUM_CHANNELS);
    return 0;
}