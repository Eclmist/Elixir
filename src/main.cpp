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

#define EXR_QUALITY_PREVIEW

#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

#include "stb/stb_image.h"
#include "stb/stbi_image_write.h"

#include "core/system/system.h"
#include "core/scene.h"
#include "math/vector3.h"
#include "math/ray.h"
#include "math/math.h"
#include "math/utils.h"
#include "geometry/sphere.h"
#include "camera/camera.h"
#include "material/lambertian.h"
#include "material/metallic.h"
#include "material/dielectric.h"
#include "material/diffuselight.h"

using namespace elixir;

exrVector3 SkyGradient(const Ray& r)
{
    exrVector3 sunsetRed(0.725f, 0.268f, 0.152f);
    exrVector3 sunsetBlue(0.18f, 0.296f, 0.952f);
    exrVector3 direction = r.m_Direction.Normalized();

    exrFloat t = (direction.y + 0.5f) / 1.2f;

    return exrLerp(sunsetRed / 3.0f, sunsetBlue / 4.0f, exrSaturate(t));
}

exrVector3 ShadePixel(const Ray& viewRay, const Scene& scene, int depth)
{
    PrimitiveHitInfo hit;

    if (scene.RaytraceScene(viewRay, 0.001f, viewRay.m_Distance, hit))
    {
        Ray scatteredRay;
        exrVector3 attenuation;
        exrVector3 emission = hit.material->Emit();

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
        return SkyGradient(viewRay);
    }
}

std::unique_ptr<Scene> GenerateScene()
{
    exrProfile("Generating Scene")

    std::unique_ptr<Scene> scene = std::make_unique<Scene>();

    for (int a = -SCENE_SIZE; a < SCENE_SIZE; a++)
    {
        for (int b = -SCENE_SIZE; b < SCENE_SIZE; b++)
        {
            exrFloat mat = Random::Random01();
            Point pos(a + 0.9f * Random::Random01(), 0.2f, b + 0.9f * Random::Random01());
            
            if ((pos - Point(4.0f, 0.2f, 0.0f)).Magnitude() > 0.9f)
            {
                // does not intersect center spheres
                if (mat < 0.8f) // diffuse
                {
                    exrFloat rand_x = Random::Random01();
                    exrFloat rand_y = Random::Random01();
                    exrFloat rand_z = Random::Random01();
                    scene->AddPrimitive(std::make_shared<Sphere>(pos, 0.2f, std::make_shared<Lambertian>(exrVector3(rand_x, rand_y, rand_z))));
                }
                else if (mat < 0.95f) // metallic
                {
                    exrFloat rand_x = 0.5f * (1.0f + Random::Random01());
                    exrFloat rand_y = 0.5f * (1.0f + Random::Random01());
                    exrFloat rand_z = 0.5f * (1.0f + Random::Random01());
                    scene->AddPrimitive(std::make_shared<Sphere>(pos, 0.2f, std::make_shared<Metallic>(exrVector3(rand_x, rand_y, rand_z), 0.5f * Random::Random01())));
                }
                else // glass
                {
                    exrFloat rand_x = 0.5f * (1.0f + Random::Random01());
                    exrFloat rand_y = 0.5f * (1.0f + Random::Random01());
                    exrFloat rand_z = 0.5f * (1.0f + Random::Random01());
                    scene->AddPrimitive(std::make_shared<Sphere>(pos, 0.2f, std::make_shared<Dielectric>(exrVector3(rand_x, rand_y, rand_z), 1.52f)));
                }
            }
        }
    }

    scene->AddPrimitive(std::make_shared<Sphere>(Point(0.0f, 1.0f, 0.0f), 1.0f, std::make_shared<DiffuseLight>(exrVector3(20.0f, 5.3f, 0.3f))));
    scene->AddPrimitive(std::make_shared<Sphere>(Point(4.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Metallic>(exrVector3(0.8f, 0.6f, 0.2f), 0.05f)));
    scene->AddPrimitive(std::make_shared<Sphere>(Point(-4.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Dielectric>(exrVector3(1.0f, 1.0f, 1.0f), 1.52f)));

    // Floor
    scene->AddPrimitive(std::make_shared<Sphere>(Point(0.0f, -1000.0f, 0.0f), 1000.0f, std::make_shared<Lambertian>(exrVector3(0.5f))));


    std::cout << "Scene initialized with " << scene->GetSceneSize() << " primitives " << std::endl;

    exrEndProfile()

    scene->InitializeBvh();
    return scene;
}

unsigned long TimeSinceEpochMillisec() {
    using namespace std::chrono;
    return unsigned long(system_clock::now().time_since_epoch() / milliseconds(1));
}

void FormatTime(unsigned long time, exrString& hh, exrString& mm, exrString& ss)
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

    hh = exrString(hr < 10 ? 1 : 0, '0').append(std::to_string(hr));
    mm = exrString(min < 10 ? 1 : 0, '0').append(std::to_string(min));
    ss = exrString(sec < 10 ? 1 : 0, '0').append(std::to_string(sec));
}

int main()
{
    std::cout << "Weekend Pathtracer Adventures vNaN.NaN.Nan" << std::endl;

    Random::Seed(11);
#include <time.h>

    // PPM Headers
    exrString header = "P6\n" + std::to_string(OUTPUT_WIDTH) + " " + std::to_string(OUTPUT_HEIGHT) + "\n255\n";
    
    std::vector<unsigned char> buffer(header.begin(), header.end());

    const exrVector3 resolution(OUTPUT_WIDTH, OUTPUT_HEIGHT, 0.0f);
    std::unique_ptr<Scene> scene = GenerateScene();

    exrProfile("Raytracing Scene")

    // camera
    Point position(-9.75f, 1.5f, 2.5f);
    Point lookat(0.0f);
    exrFloat fov = 40.0f;
    exrFloat aspect = exrFloat(OUTPUT_WIDTH) / exrFloat(OUTPUT_HEIGHT);
    exrFloat focusDist = (position - lookat).Magnitude();
    exrFloat aperture = 0.05f;
    Camera camera(position, lookat, exrVector3::Up(), fov, aspect, aperture, focusDist);

    unsigned long lastTime = TimeSinceEpochMillisec();
    unsigned long avgTimePerRow;

    for (int y = OUTPUT_HEIGHT - 1; y >= 0; y--)
    {
        for (int x = 0; x < OUTPUT_WIDTH; x++)
        {
            exrVector3 color = exrVector3::Zero();

            // Get samples
            for (int i = 0; i < NUM_SAMPLES_PER_PIXEL; i++)
            {
                exrFloat u = exrFloat(x + Random::Random01()) / exrFloat(OUTPUT_WIDTH);
                exrFloat v = exrFloat(y + Random::Random01()) / exrFloat(OUTPUT_HEIGHT);
                Ray viewRay = camera.GetViewRay(u, v);

                color += ShadePixel(viewRay, *scene, NUM_BOUNDCE_PER_RAY);
            }

            color /= NUM_SAMPLES_PER_PIXEL;

            // Correct gamma
            color = exrVector3(sqrt(color.x), sqrt(color.y), sqrt(color.z));

            // Clamp color values for ppm
            color.x = exrSaturate(color.x);
            color.y = exrSaturate(color.y);
            color.z = exrSaturate(color.z);

            // PPM takes values from 0-255;
            color *= 255.99f;
            
            unsigned char r = static_cast<unsigned char>(color.r);
            unsigned char g = static_cast<unsigned char>(color.g);
            unsigned char b = static_cast<unsigned char>(color.b);

            buffer.push_back(r);
            buffer.push_back(g);
            buffer.push_back(b);
        }

        exrFloat progress = 100.0f - (exrFloat(y) / OUTPUT_HEIGHT * 100.0f);
        auto newTime = TimeSinceEpochMillisec();

        if (y == OUTPUT_HEIGHT - 1)
            avgTimePerRow = newTime - lastTime;

        avgTimePerRow = (avgTimePerRow + (newTime - lastTime)) / 2;
        unsigned long timeLeft = avgTimePerRow * size_t(y);
        lastTime = newTime;

        exrString progressBar = "[";
        for (int i = 0; i < 100; i+= 3)
        {
            if (i < progress)
                progressBar += "=";
            else
                progressBar += " ";
        }
        progressBar += "] ";

        exrString hh, mm, ss;
        FormatTime(timeLeft, hh, mm, ss);
        exrInfo("Progress: " << progressBar << "\t" << int(progress) << "% ETA: " << hh << ":" << mm << ":" << ss << "            " << '\r');
    }
    exrEndProfile()

    int x, y, n;
    stbi_uc* output = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size()), &x, &y, &n, 0);
    stbi_write_png("output.png", OUTPUT_WIDTH, OUTPUT_HEIGHT, NUM_CHANNELS, output, OUTPUT_WIDTH * NUM_CHANNELS);

    system("PAUSE");
    system("output.png");
    return 0;
}
