#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stbi_image_write.h"

int main()
{
    const int nx = 200;
    const int ny = 100;
    const unsigned int outputSize = nx * ny * 3;

    char output[outputSize];

    for (int y = 0; y < ny; y++)
    {
        for (int x = 0; x < nx; x++)
        {
            float r = float(x) / float(nx);
            float g = float(y) / float(ny);
            float b = 1.0f;

            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);

            output[y * ny + (x * 3)] = ir;
            output[y * ny + (x * 3 + 1)] = ir;
            output[y * ny + (x * 3 + 2)] = ir;
        }
    }

    stbi_write_png("out.png", nx, ny, 3, output, nx * 3);
}