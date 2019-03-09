#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "stb\stb_image.h"
#include "stb\stbi_image_write.h"

#include "math/vector3.h"
#include "math/ray.h"
#include "math/utils.h"

#define OUTPUT_WIDTH 800
#define OUTPUT_HEIGHT 460
#define NUM_CHANNELS 3

Vector3f SkyGradient(const Ray& r)
{
    Vector3f skyBlue(0.5f, 0.7f, 1.0f);
    Vector3f white(1.0f);
    Vector3f direction = r.GetDirection().Normalized();

    float t = 0.5f * direction.y + 1.0f;

    return LERP(white, skyBlue, t);
}

int main()
{
    // PPM Headers
    std::string header = "P6\n" + std::to_string(OUTPUT_WIDTH) + " " + std::to_string(OUTPUT_HEIGHT) + "\n255\n";
    
    std::vector<unsigned char> buffer(header.begin(), header.end());

    const Vector3f resolution(OUTPUT_WIDTH, OUTPUT_HEIGHT, 0.0f);

    for (int y = OUTPUT_HEIGHT - 1; y >= 0; y--)
    {
        for (int x = 0; x < OUTPUT_WIDTH; x++)
        {
            const Vector3f fragCoord(float(x), float(y), 0.0f);
            const Vector3f uv = (fragCoord - 0.5f * resolution) / resolution.y;

            // View ray
            const Vector3f origin = Vector3f::Zero();
            const Vector3f direction = Vector3f(uv.x, uv.y, 1.0f).Normalized();
            const Ray viewRay(origin, direction, 1000.0f);

            Vector3f color = SkyGradient(viewRay);


            // Clamp color values for ppm
            color.x = SATURATE(color.x);
            color.y = SATURATE(color.y);
            color.z = SATURATE(color.z);

            // PPM takes values from 0-255;
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

    system("output.png");
    return 0;
}