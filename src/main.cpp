#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "stb/stb_image.h"
#include "stb/stbi_image_write.h"

#include "math/vector3.h"
#include "math/ray.h"
#include "math/random.h"
#include "math/utils.h"
#include "core/scene.h"
#include "core/timer.h"
#include "geometrics/sphere.h"
#include "camera/camera.h"
#include "material/lambertian.h"
#include "material/metallic.h"
#include "material/dielectric.h"

#define QUALITY_SETTING_HIGH

#ifdef QUALITY_SETTING_ULTRA
#define OUTPUT_WIDTH 1000
#define OUTPUT_HEIGHT 600
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 16
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 9
#endif
#ifdef QUALITY_SETTING_HIGH
#define OUTPUT_WIDTH 1000
#define OUTPUT_HEIGHT 600
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 8
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 9
#endif
#ifdef QUALITY_SETTING_MEDIUM
#define OUTPUT_WIDTH 500
#define OUTPUT_HEIGHT 300
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 4
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 7
#endif
#ifdef QUALITY_SETTING_LOW
#define OUTPUT_WIDTH 250
#define OUTPUT_HEIGHT 150
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 2
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 7
#endif
#ifdef QUALITY_SETTING_PREVIEW
#define OUTPUT_WIDTH 250
#define OUTPUT_HEIGHT 150
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 1
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 7
#endif

Vector3f SkyGradient(const Ray& r)
{
    Vector3 skyBlue(0.5f, 0.7f, 1.0f);
    Vector3 white(1.0f);
    Vector3 direction = r.m_Direction.Normalized();

    float t = 0.5f * direction.y + 1.0f;

    return LERP(white, skyBlue, t);
}

Vector3f ShadePixel(const Ray& viewRay, const Scene& scene, int depth)
{
    PrimitiveHitInfo hit;

    // max bounce reached
    if (depth == 0)
        return Vector3(0.0f); // assume no light at all

    if (scene.RaytraceScene(viewRay, 0.001f, viewRay.m_Distance, hit))
    {
        Ray scatteredRay;
        Vector3f attenuation;

        if (hit.material->Scatter(viewRay, hit, attenuation, scatteredRay))
        {
            // recursively collect color contribution
            return attenuation * ShadePixel(scatteredRay, scene, depth - 1);
        }
        else
        {
            // If scattered ray did not intersect with any object, assume we hit sky
            return Vector3(0.0f);
        }
    }
    else
    {
        return SkyGradient(viewRay);
    }
}

std::unique_ptr<Scene> GenerateScene()
{
    TIMER_PROFILE_CPU("Generating Scene")

    std::unique_ptr<Scene> scene = std::make_unique<Scene>();

    for (int a = -SCENE_SIZE; a < SCENE_SIZE; a++)
    {
        for (int b = -SCENE_SIZE; b < SCENE_SIZE; b++)
        {
            float mat = Random::Random01();
            Point pos(a + 0.9f * Random::Random01(), 0.2f, b + 0.9f * Random::Random01());
            
            if ((pos - Point(4.0f, 0.2f, 0.0f)).Magnitude() > 0.9f)
            {
                // does not intersect center spheres
                if (mat < 0.8f) // diffuse
                {
                    float rand_x = Random::Random01();
                    float rand_y = Random::Random01();
                    float rand_z = Random::Random01();
                    scene->AddPrimitive(std::make_shared<Sphere>(pos, 0.2f, std::make_shared<Lambertian>(Vector3(rand_x, rand_y, rand_z))));
                }
                else if (mat < 0.95f) // metallic
                {
                    float rand_x = 0.5f * (1.0f + Random::Random01());
                    float rand_y = 0.5f * (1.0f + Random::Random01());
                    float rand_z = 0.5f * (1.0f + Random::Random01());
                    scene->AddPrimitive(std::make_shared<Sphere>(pos, 0.2f, std::make_shared<Metallic>(Vector3(rand_x, rand_y, rand_z), 0.5f * Random::Random01())));
                }
                else // glass
                {
                    float rand_x = 0.5f * (1.0f + Random::Random01());
                    float rand_y = 0.5f * (1.0f + Random::Random01());
                    float rand_z = 0.5f * (1.0f + Random::Random01());
                    scene->AddPrimitive(std::make_shared<Sphere>(pos, 0.2f, std::make_shared<Dielectric>(Vector3(rand_x, rand_y, rand_z), 1.52f)));
                }
            }
        }
    }

    scene->AddPrimitive(std::make_shared<Sphere>(Point(0.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Lambertian>(Vector3(0.8f, 0.3f, 0.3f))));
    scene->AddPrimitive(std::make_shared<Sphere>(Point(4.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Metallic>(Vector3(0.8f, 0.6f, 0.2f), 0.05f)));
    scene->AddPrimitive(std::make_shared<Sphere>(Point(-4.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Dielectric>(Vector3(1.0f, 1.0f, 1.0f), 1.52f)));

    // Floor
    scene->AddPrimitive(std::make_shared<Sphere>(Point(0.0f, -1000.0f, 0.0f), 1000.0f, std::make_shared<Lambertian>(Vector3(0.5f))));


    std::cout << "Scene initialized with " << scene->GetSceneSize() << " primitives " << std::endl;

    TIMER_ENDPROFILE_CPU()

    scene->InitializeBvh();
    return scene;
}

int main()
{
    std::cout << "Weekend Pathtracer Adventures vNaN.NaN.Nan" << std::endl;

    Random::Seed(11);

    // PPM Headers
    std::string header = "P6\n" + std::to_string(OUTPUT_WIDTH) + " " + std::to_string(OUTPUT_HEIGHT) + "\n255\n";
    
    std::vector<unsigned char> buffer(header.begin(), header.end());

    const Vector3f resolution(OUTPUT_WIDTH, OUTPUT_HEIGHT, 0.0f);
    std::unique_ptr<Scene> scene = GenerateScene();

    TIMER_PROFILE_CPU("Raytracing Scene")

    // camera
    Point position(-9.75f, 1.5f, 2.5f);
    Point lookat(0.0f);
    float fov = 40.0f;
    float aspect = float(OUTPUT_WIDTH) / float(OUTPUT_HEIGHT);
    float focusDist = (position - lookat).Magnitude();
    float aperture = 0.05f;
    Camera camera(position, lookat, Vector3f::Up(), fov, aspect, aperture, focusDist);

    for (int y = OUTPUT_HEIGHT - 1; y >= 0; y--)
    {
        for (int x = 0; x < OUTPUT_WIDTH; x++)
        {
            Vector3 color = Vector3f::Zero();

            // Get samples
            for (int i = 0; i < NUM_SAMPLES_PER_PIXEL; i++)
            {
                float u = float(x + Random::Random01()) / float(OUTPUT_WIDTH);
                float v = float(y + Random::Random01()) / float(OUTPUT_HEIGHT);
                Ray viewRay = camera.GetViewRay(u, v);

                color += ShadePixel(viewRay, *scene, NUM_BOUNDCE_PER_RAY);
            }

            color /= NUM_SAMPLES_PER_PIXEL;

            // Correct gamma
            color = Vector3(sqrt(color.x), sqrt(color.y), sqrt(color.z));

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

        int progress = int(100.0f - (float(y) / OUTPUT_HEIGHT * 100.0f));

        std::string progressBar = "[";
        for (int i = 0; i < 100; i+= 3)
        {
            if (i < progress)
                progressBar += "=";
            else
                progressBar += " ";
        }
        progressBar += "] ";
        
        std::cout << "Progress: " << progressBar << "\t" << progress << "%" << '\r';
    }
    std::cout << std::endl;
    TIMER_ENDPROFILE_CPU()

    int x, y, n;
    stbi_uc* output = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size()), &x, &y, &n, 0);
    stbi_write_png("output.png", OUTPUT_WIDTH, OUTPUT_HEIGHT, NUM_CHANNELS, output, OUTPUT_WIDTH * NUM_CHANNELS);

    system("PAUSE");
    system("output.png");
    return 0;
}