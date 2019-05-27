/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019 Samuel Van Allen - All rights reserved.

    Elixir is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* ========================================================================================== /

         ######\  ##\      ########\  ######\  ##\   ##\       ##\      ##\ ########\
        ##  __##\ ## |     ##  _____|##  __##\ ###\  ## |      ###\    ### |##  _____|
        ## /  \__|## |     ## |      ## /  ## |####\ ## |      ####\  #### |## |
        ## |      ## |     #####\    ######## |## ##\## |      ##\##\## ## |#####\
        ## |      ## |     ##  __|   ##  __## |## \#### |      ## \###  ## |##  __|
        ## |  ##\ ## |     ## |      ## |  ## |## |\### |      ## |\#  /## |## |
        \######  |########\########\ ## |  ## |## | \## |      ## | \_/ ## |########\
         \______/ \________\________|\__|  \__|\__|  \__|      \__|     \__|\________|

             Banner of shame. Remove when this file is no longer ghetto. 
 =========================================================================================== */

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#define EXR_QUALITY_LEVEL				3

#include <thread>
#include "stb/stb_image.h"
#include "stb/stbi_image_write.h"

#include "core/elixir.h"
#include "core/scene.h"
#include "camera/camera.h"
#include "material/lambertian.h"
#include "material/metallic.h"
#include "material/dielectric.h"
#include "material/diffuselight.h"
#include "geometry/sphere.h"
#include "geometry/quad.h"

exrBEGIN_NAMESPACE

exrVector3 SkyGradient(const Ray& r)
{
    exrVector3 sunsetRed(0.725f, 0.268f, 0.152f);
    exrVector3 sunsetBlue(0.18f, 0.296f, 0.952f);
    exrVector3 direction = r.m_Direction.Normalized();

    exrFloat t = (direction.y + 0.5f) / 1.2f;

    return exrLerp(sunsetRed, sunsetBlue, exrSaturate(t));
}

exrVector3 ShadePixel(const Ray& viewRay, const Scene& scene, int depth)
{
    PrimitiveHitInfo hit;

    if (scene.RaytraceScene(viewRay, 0.001f, viewRay.m_Distance, hit))
    {
        Ray scatteredRay;
        exrVector3 attenuation;
        exrVector3 emission = hit.m_Material->Emit();

        if (depth > 0 && hit.m_Material->Scatter(viewRay, hit, attenuation, scatteredRay))
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
        return exrVector3::Zero();
        //return SkyGradient(viewRay);
    }
}

std::unique_ptr<Scene> GenerateScene()
{
    exrProfile("Generating Scene")

    std::unique_ptr<Scene> scene = std::make_unique<Scene>();

    // left
    scene->AddPrimitive(std::make_unique<Quad>(exrPoint(-2.5f, 2.5f, 0.0f), exrVector2(5.0f), exrVector3(0, exrDegToRad(90.0f), 0), std::make_unique<Lambertian>(exrVector3(1.0f, 0.0f, 0.0f))));
    // right
    scene->AddPrimitive(std::make_unique<Quad>(exrPoint(2.5f, 2.5f, 0.0f), exrVector2(5.0f), exrVector3(0, exrDegToRad(-90.0f), 0), std::make_unique<Lambertian>(exrVector3(0.0f, 0.5f, 1.0f))));
    // back
    scene->AddPrimitive(std::make_unique<Quad>(exrPoint(0.0f, 2.5f, -2.5f), exrVector2(5.0f), exrVector3(0.0f), std::make_unique<Lambertian>(exrVector3(1.0f))));
    // floor
    scene->AddPrimitive(std::make_unique<Quad>(exrPoint(0.0f), exrVector2(5.0f), exrVector3(exrDegToRad(-90), 0, 0), std::make_unique<Lambertian>(exrVector3(1.0f))));
    // ceiling
    scene->AddPrimitive(std::make_unique<Quad>(exrPoint(0.0f, 5.0f, 0.0f), exrVector2(5.0f), exrVector3(exrDegToRad(90), 0, 0), std::make_unique<Lambertian>(exrVector3(1.0f))));
    // light
    scene->AddPrimitive(std::make_unique<Quad>(exrPoint(0.0f, 5.0f, 0.0f), exrVector2(1.0f), exrVector3(exrDegToRad(90), 0, 0), std::make_unique<DiffuseLight>(exrVector3(1.0f, 0.9f, 0.7f) * 15.0f)));

    scene->AddPrimitive(std::make_unique<Sphere>(exrPoint(0.0f, 2.5f, 0.0f), 1.0f, std::make_unique<Dielectric>(exrVector3(1.0f, 1.0f, 1.0f), 1.5f)));
    scene->AddPrimitive(std::make_unique<Sphere>(exrPoint(-1.0f, 0.6f, 0.0f), 1.4f, std::make_unique<Lambertian>(exrVector3(1.0f, 1.0f, 1.0f))));
    exrEndProfile()
    exrInfoLine("Scene initialized with " << scene->GetSceneSize() << " primitives")

    scene->InitializeBvh();
    return scene;
}

stbi_uc* Render()
{
    Random::Seed(11);

    // PPM Headers
    exrString header = "P6\n" + std::to_string(OUTPUT_WIDTH) + " " + std::to_string(OUTPUT_HEIGHT) + "\n255\n";
    
    std::vector<unsigned char> buffer(header.begin(), header.end());

    const auto data_offset = buffer.size();
    buffer.resize(buffer.size() + OUTPUT_HEIGHT * OUTPUT_WIDTH * sizeof(unsigned char) * 3);
    memcpy(buffer.data(), header.c_str(), strlen(header.c_str()));

    const exrVector3 resolution(OUTPUT_WIDTH, OUTPUT_HEIGHT, 0.0f);
    std::unique_ptr<Scene> scene = GenerateScene();

    exrProfile("Raytracing Scene")

    // camera
    exrPoint position(0.0f, 2.5f, 9.0f);
    exrPoint lookat(0.0f, 2.5f, 0.0f);
    exrFloat fov = 40.0f;
    exrFloat aspect = exrFloat(OUTPUT_WIDTH) / exrFloat(OUTPUT_HEIGHT);
    exrFloat focusDist = (position - lookat).Magnitude();
    exrFloat aperture = 0.05f;
    Camera camera(position, lookat, exrVector3::Up(), fov, aspect, aperture, focusDist);

    exrU64 lastTime = Timer::TimeSinceEpochMillisec();
    exrU64 avgTimePerRow;

    auto shade_pixel = [&](int x, int y){
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

        const auto offset = 3 * ((OUTPUT_HEIGHT - 1 - y)* OUTPUT_WIDTH + x) + data_offset;
        buffer[offset] = r;
        buffer[offset + 1] = g;
        buffer[offset + 2] = b;
    };

    constexpr unsigned THREAD_COUNT = 12;
    auto rendering_trunk = [&](int tid){
        const unsigned TRUNK_SIZE = (OUTPUT_HEIGHT + THREAD_COUNT - 1) / THREAD_COUNT;
        const unsigned trunk_end = OUTPUT_HEIGHT < (tid + 1) * TRUNK_SIZE ? OUTPUT_HEIGHT : (tid + 1) * TRUNK_SIZE;
        for( unsigned y = tid * TRUNK_SIZE ; y < trunk_end ; ++y )
            for (int x = 0; x < OUTPUT_WIDTH; x++)
                shade_pixel(x, y);
    };

    std::thread rendering_threads[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; ++i)
        rendering_threads[i] = std::thread(rendering_trunk, i);
    
    for (int i = 0; i < THREAD_COUNT; ++i)
        rendering_threads[i].join();

    /*
    for (int y = OUTPUT_HEIGHT - 1; y >= 0; y--)
    {
        for (int x = 0; x < OUTPUT_WIDTH; x++)
        {
            shade_pixel(x, y);
        }

        /*
        exrFloat progress = 100.0f - (exrFloat(y) / OUTPUT_HEIGHT * 100.0f);
        auto newTime = Timer::TimeSinceEpochMillisec();

        if (y == OUTPUT_HEIGHT - 1)
            avgTimePerRow = newTime - lastTime;

        avgTimePerRow = (avgTimePerRow + (newTime - lastTime)) / 2;
        exrU64 timeLeft = avgTimePerRow * size_t(y);
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
        Timer::FormatTime(timeLeft, hh, mm, ss);
        exrInfo("Progress: " << progressBar << "\t" << int(progress) << "% ETA: " << hh << ":" << mm << ":" << ss << "            " << '\r');
    }
    */
    exrEndProfile()

    int x, y, n;
    return stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size()), &x, &y, &n, 0);
}

exrEND_NAMESPACE

using namespace elixir;

int main()
{
    exrInfoLine("Elixir Version " << EXR_VERSION_MAJOR << "." << EXR_VERSION_MINOR << "." << EXR_VERSION_PATCH)

    stbi_uc* output = Render();

    exrString fileName = "output_" + std::to_string(Timer::TimeSinceEpochMillisec()) + "_" + std::to_string(NUM_SAMPLES_PER_PIXEL) + "spp_" + std::to_string(NUM_BOUNDCE_PER_RAY) + "b.png";
    stbi_write_png(fileName.c_str(), OUTPUT_WIDTH, OUTPUT_HEIGHT, NUM_CHANNELS, output, OUTPUT_WIDTH * NUM_CHANNELS);

#ifdef EXR_PLATFORM_WIN
    system("PAUSE");
    system(fileName.c_str());
#endif

    return 0;
}
