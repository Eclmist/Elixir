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

// Temp objects until we set up proper scene management
Vector3f SkyGradient(const Ray& r)
{
    Vector3f skyBlue(0.5f, 0.7f, 1.0f);
    Vector3f white(1.0f);
    Vector3f direction = r.GetDirection().Normalized();

    float t = 0.5f * direction.y + 1.0f;

    return LERP(white, skyBlue, t);
}

float RaycastSphere(const Ray& ray, const Vector3f& center, float radius)
{
    Vector3 oc = ray.GetOrigin() - center;
    float a = Dot(ray.GetDirection(), ray.GetDirection());
    float b = 2.0f * Dot(oc, ray.GetDirection());
    float c = Dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    // solve quadratic equation to find t
    if (discriminant < 0)
    {
        // no real value, did not intersect sphere
        return -1.0f;
    }
    else
    {
        return (-b - sqrt(discriminant) / (2.0f * a));
    }
}

Vector3f ShadePixel(const Ray& viewRay)
{
    Vector3f spherePosition = Vector3f(0.0f, 0.0f, 3.0f);
    float sphereIntersect = RaycastSphere(viewRay, spherePosition, 1.0f);

    if (sphereIntersect > 0.0f)
    {
        Vector3f normals = (viewRay.Evaluate(sphereIntersect) - spherePosition).Normalized();
        return 0.5f * Vector3(normals.x + 1, normals.y + 1, normals.z + 1);
    }
    else
    {
        return SkyGradient(viewRay);
    }
}

// ! Temp object

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
            const Vector3f uv = (fragCoord - 0.5f * resolution) / resolution.y * 2.0f;

            // View ray
            const Vector3f origin = Vector3f::Zero();
            const Vector3f direction = Vector3f(uv.x, uv.y, 1.0f).Normalized();
            const Ray viewRay(origin, direction, 1000.0f);

            Vector3f color = ShadePixel(viewRay);

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