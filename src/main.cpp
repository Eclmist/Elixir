#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "extern\stb\stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "extern\stb\stbi_image_write.h"

#include "math/vector3.h"

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
            Vector3f color(static_cast<float>(x) / OUTPUT_WIDTH, static_cast<float>(y) / OUTPUT_HEIGHT, 0.5f);
            color *= 255.99f;

            unsigned char r = static_cast<unsigned char>(color.r);
            unsigned char g = static_cast<unsigned char>(color.g);
            unsigned char b = static_cast<unsigned char>(color.b);

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