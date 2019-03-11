#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "stb/stb_image.h"
#include "stb/stbi_image_write.h"

#include "math/vector3.h"
#include "math/ray.h"
#include "math/utils.h"
#include "core/scene.h"
#include "geometrics/sphere.h"
#include "camera/camera.h"
#include "material/lambertian.h"
#include "material/metallic.h"
#include "material/dielectric.h"

#define OUTPUT_WIDTH 800
#define OUTPUT_HEIGHT 480
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 64
#define NUM_BOUNDCE_PER_RAY 8

Vector3f SkyGradient(const Ray& r)
{
    Vector3f skyBlue(0.5f, 0.7f, 1.0f);
    Vector3f white(1.0f);
    Vector3f direction = r.m_Direction.Normalized();

    float t = 0.5f * direction.y + 1.0f;

    return LERP(white, skyBlue, t);
}

Vector3f ShadePixel(const Ray& viewRay, const Scene& scene, int depth)
{
    GeometryHitInfo hit;

    // max bounce reached
    if (depth == 0)
        return Vector3f(0.0f); // assume no light at all

    if (scene.RaytraceScene(viewRay, 0.001f, viewRay.m_Distance, hit))
    {
        Ray scatteredRay;
        Vector3f attenuation;

        if (hit.pMaterial->Scatter(viewRay, hit, attenuation, scatteredRay))
        {
            // recursively collect color contribution
            return attenuation * ShadePixel(scatteredRay, scene, depth - 1);
        }
        else
        {
            // If scattered ray did not intersect with any object, assume we hit sky
            return SkyGradient(viewRay);
        }
    }
    else
    {
        return SkyGradient(viewRay);
    }
}

Scene* GenerateScene()
{
    Scene* scene = new Scene();

    for (int a = -9; a < 9; a++)
    {
        for (int b = -9; b < 9; b++)
        {
            float mat = RAND01();
            Vector3f pos(a + 0.9f * RAND01(), 0.2f, b + 0.9f * RAND01());

            if ((pos - Vector3f(4.0f, 0.2f, 0.0f)).Magnitude() > 0.9f)
            {
                // does not intersect center spheres
                if (mat < 0.8f) // diffuse
                {
                    float rand_x = RAND01();
                    float rand_y = RAND01();
                    float rand_z = RAND01();
                    scene->AddPrimitive(new Sphere(pos, 0.2f, new Lambertian(Vector3f(rand_x, rand_y, rand_z))));
                }
                else if (mat < 0.95f) // metallic
                {
                    float rand_x = 0.5f * (1.0f + RAND01());
                    float rand_y = 0.5f * (1.0f + RAND01());
                    float rand_z = 0.5f * (1.0f + RAND01());
                    scene->AddPrimitive(new Sphere(pos, 0.2f, 
                        new Metallic(Vector3f(rand_x, rand_y, rand_z), 0.5f * RAND01())));
                }
                else // glass
                {
                    float rand_x = 0.5f * (1.0f + RAND01());
                    float rand_y = 0.5f * (1.0f + RAND01());
                    float rand_z = 0.5f * (1.0f + RAND01());
                    scene->AddPrimitive(new Sphere(pos, 0.2f, new Dielectric(Vector3f(rand_x, rand_y, rand_z), 1.52f)));
                }
            }
        }
    }


    scene->AddPrimitive(new Sphere(Vector3f(0.0f, 1.0f, 0.0f), 1.0f, new Lambertian(Vector3f(0.8f, 0.3f, 0.3f))));
    scene->AddPrimitive(new Sphere(Vector3f(4.0f, 1.0f, 0.0f), 1.0f, new Metallic(Vector3f(0.8f, 0.6f, 0.2f), 0.05f)));
    scene->AddPrimitive(new Sphere(Vector3f(-4.0f, 1.0f, 0.0f), 1.0f, new Dielectric(Vector3f(0.8f, 0.7f, 1.0f), 1.52f)));

    // Floor
    scene->AddPrimitive(new Sphere(Vector3f(0.0f, -1000.0f, 0.0f), 1000.0f, new Lambertian(Vector3f(0.5f))));

    return scene;
}

int main()
{

    // PPM Headers
    std::string header = "P6\n" + std::to_string(OUTPUT_WIDTH) + " " + std::to_string(OUTPUT_HEIGHT) + "\n255\n";
    
    std::vector<unsigned char> buffer(header.begin(), header.end());

    const Vector3f resolution(OUTPUT_WIDTH, OUTPUT_HEIGHT, 0.0f);
    Scene* scene = GenerateScene();

    // camera
    Vector3f position(-9.75f, 1.5f, 2.5f);
    Vector3f lookat(0.0f);
    float fov = 30.0f;
    float aspect = float(OUTPUT_WIDTH) / float(OUTPUT_HEIGHT);
    float focusDist = (position - lookat).Magnitude();
    float aperture = 0.02f;
    Camera camera(position, lookat, Vector3f::Up(), fov, aspect, aperture, focusDist);

    for (int y = OUTPUT_HEIGHT - 1; y >= 0; y--)
    {
        for (int x = 0; x < OUTPUT_WIDTH; x++)
        {
            Vector3f color = Vector3f::Zero();

            // Get samples
            for (int i = 0; i < NUM_SAMPLES_PER_PIXEL; i++)
            {
                float u = float(x + RAND01()) / float(OUTPUT_WIDTH);
                float v = float(y + RAND01()) / float(OUTPUT_HEIGHT);
                Ray viewRay = camera.GetViewRay(u, v);
                color += ShadePixel(viewRay, *scene, NUM_BOUNDCE_PER_RAY);
            }

            color /= NUM_SAMPLES_PER_PIXEL;

            // Correct gamma
            color = Vector3f(sqrt(color.x), sqrt(color.y), sqrt(color.z));

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

        std::cout << "Progress: " << 100.0f - (float(y) / OUTPUT_HEIGHT * 100.0f) << "%" << std::endl;
    }

    int x, y, n;
    stbi_uc* output = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size()), &x, &y, &n, 0);
    stbi_write_png("output.png", OUTPUT_WIDTH, OUTPUT_HEIGHT, NUM_CHANNELS, output, OUTPUT_WIDTH * NUM_CHANNELS);

    system("output.png");
    return 0;
}