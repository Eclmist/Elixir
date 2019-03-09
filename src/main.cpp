#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "stb/stb_image.h"
#include "stb/stbi_image_write.h"

#include "math/vector3.h"
#include "math/ray.h"
#include "math/utils.h"
#include "core/scene.h"
#include "geometrics/sphere.h"
#include "camera/camera.h"

#define OUTPUT_WIDTH 800
#define OUTPUT_HEIGHT 400
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 4

Vector3f SkyGradient(const Ray& r)
{
    Vector3f skyBlue(0.5f, 0.7f, 1.0f);
    Vector3f white(1.0f);
    Vector3f direction = r.m_Direction.Normalized();

    float t = 0.5f * direction.y + 1.0f;

    return LERP(white, skyBlue, t);
}

Vector3f ShadePixel(const Ray& viewRay, const Scene& scene)
{
    RayHitRecord hit;

    if (scene.RaytraceScene(viewRay, hit))
    {
        Vector3f normals = hit.normal;
        return 0.5f * Vector3(normals.x + 1, normals.y + 1, normals.z + 1);
    }
    else
    {
        return SkyGradient(viewRay);
    }
}

Scene* GenerateScene()
{
    Scene* scene = new Scene();

    scene->AddPrimitive(new Sphere(Vector3f(0.0f, 0.0f, -1.0f), 0.5f));
    scene->AddPrimitive(new Sphere(Vector3f(0.0f, -100.5f, -1.0f), 100.0f));

    return scene;
}

int main()
{
    // PPM Headers
    std::string header = "P6\n" + std::to_string(OUTPUT_WIDTH) + " " + std::to_string(OUTPUT_HEIGHT) + "\n255\n";
    
    std::vector<unsigned char> buffer(header.begin(), header.end());

    const Vector3f resolution(OUTPUT_WIDTH, OUTPUT_HEIGHT, 0.0f);
    Scene* scene = GenerateScene();
    Camera camera;

    for (int y = OUTPUT_HEIGHT - 1; y >= 0; y--)
    {
        for (int x = 0; x < OUTPUT_WIDTH; x++)
        {
            Vector3f color = Vector3f::Zero();

            // Get samples
            for (int i = 0; i < NUM_SAMPLES_PER_PIXEL; i++)
            {
                float rand_u = (float)rand() / RAND_MAX;
                float rand_v = (float)rand() / RAND_MAX;
                float u = float(x + rand_u) / float(OUTPUT_WIDTH);
                float v = float(y + rand_v) / float(OUTPUT_HEIGHT);
                Ray viewRay = camera.GetViewRay(u, v);
                color += ShadePixel(viewRay, *scene);
            }

            color /= NUM_SAMPLES_PER_PIXEL;

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