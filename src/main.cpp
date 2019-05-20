/* ================================================================================================ /
 ____    ____  ____   ____     ___  ____        ___   _____       _____ __ __   ____  ___ ___    ___
|    \  /    ||    \ |    \   /  _]|    \      /   \ |     |     / ___/|  |  | /    ||   |   |  /  _]
|  o  )|  o  ||  _  ||  _  | /  [_ |  D  )    |     ||   __|    (   \_ |  |  ||  o  || _   _ | /  [_
|     ||     ||  |  ||  |  ||    _]|    /     |  O  ||  |_       \__  ||  _  ||     ||  \_/  ||    _]
|  O  ||  _  ||  |  ||  |  ||   [_ |    \     |     ||   _]      /  \ ||  |  ||  _  ||   |   ||   [_
|     ||  |  ||  |  ||  |  ||     ||  .  \    |     ||  |        \    ||  |  ||  |  ||   |   ||     |
|_____||__|__||__|__||__|__||_____||__|\_|     \___/ |__|         \___||__|__||__|__||___|___||_____|

                       This file is ghetto. Remove when no longer ghetto. 
 ================================================================================================ */


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

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
#include "material/diffuselight.h"

#define QUALITY_SETTING_ULTRA

#ifdef QUALITY_SETTING_ULTRA
#define OUTPUT_WIDTH 1000
#define OUTPUT_HEIGHT 600
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 512
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 1
#endif
#ifdef QUALITY_SETTING_HIGH
#define OUTPUT_WIDTH 1000
#define OUTPUT_HEIGHT 600
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 8
#define NUM_BOUNDCE_PER_RAY 4int
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

    if (scene.RaytraceScene(viewRay, 0.001f, viewRay.m_Distance, hit))
    {
#include <time.h>
        Ray scatteredRay;
        Vector3f attenuation;
        Vector3f emission = hit.material->Emit();

        if (depth > 0 && hit.material->Scatter(viewRay, hit, attenuation, scatteredRay))
        {
            // recursively collect color contribution
            return emission + attenuation * ShadePixel(scatteredRay, scene, depth - 1);
        }
        else
        {
            // If scattered ray did not intersect with any object, 
            return emission;
        }
    }
    else
    {
        return Vector3(0.0f); // SkyGradient(viewRay);
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

    scene->AddPrimitive(std::make_shared<Sphere>(Point(0.0f, 1.0f, 0.0f), 1.0f, std::make_shared<DiffuseLight>(Vector3(20.0f, 5.3f, 0.3f))));
    scene->AddPrimitive(std::make_shared<Sphere>(Point(4.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Metallic>(Vector3(0.8f, 0.6f, 0.2f), 0.05f)));
    scene->AddPrimitive(std::make_shared<Sphere>(Point(-4.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Dielectric>(Vector3(1.0f, 1.0f, 1.0f), 1.52f)));

    // Floor
    scene->AddPrimitive(std::make_shared<Sphere>(Point(0.0f, -1000.0f, 0.0f), 1000.0f, std::make_shared<Lambertian>(Vector3(0.5f))));


    std::cout << "Scene initialized with " << scene->GetSceneSize() << " primitives " << std::endl;

    TIMER_ENDPROFILE_CPU()

    scene->InitializeBvh();
    return scene;
}

unsigned long TimeSinceEpochMillisec() {
    using namespace std::chrono;
    return unsigned long(system_clock::now().time_since_epoch() / milliseconds(1));
}

void FormatTime(unsigned long time, std::string& hh, std::string& mm, std::string& ss)
{
    //3600000 milliseconds in an hour
    long hr = time / 3600000;
    time = time - 3600000 * hr;
    //60000 milliseconds in a minute
    long min = time / 60000;
    time = time - 60000 * min;

    //1000 milliseconds in a second
    long sec = time / 1000;
    time = time - 1000 * sec;

    hh = std::string(hr < 10 ? 1 : 0, '0').append(std::to_string(hr));
    mm = std::string(min < 10 ? 1 : 0, '0').append(std::to_string(min));
    ss = std::string(sec < 10 ? 1 : 0, '0').append(std::to_string(sec));
}

int main()
{
    std::cout << "Weekend Pathtracer Adventures vNaN.NaN.Nan" << std::endl;

    Random::Seed(11);
#include <time.h>

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

    unsigned long lastTime = TimeSinceEpochMillisec();
    unsigned long avgTimePerRow;

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

#include <time.h>
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

        float progress = 100.0f - (float(y) / OUTPUT_HEIGHT * 100.0f);
        auto newTime = TimeSinceEpochMillisec();

        if (y == OUTPUT_HEIGHT - 1)
            avgTimePerRow = newTime - lastTime;

        avgTimePerRow = (avgTimePerRow + (newTime - lastTime)) / 2;
        unsigned long timeLeft = avgTimePerRow * size_t(y);
        lastTime = newTime;

        std::string progressBar = "[";
        for (int i = 0; i < 100; i+= 3)
        {
            if (i < progress)
                progressBar += "=";
            else
                progressBar += " ";
        }
        progressBar += "] ";

        std::string hh, mm, ss;
        FormatTime(timeLeft, hh, mm, ss);
        std::cout << "Progress: " << progressBar << "\t" << int(progress) << "% ETA: " << hh << ":" << mm << ":" << ss << "            " << '\r';
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