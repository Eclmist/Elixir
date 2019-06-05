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

#define OUTPUT_WIDTH 512
#define OUTPUT_HEIGHT 512
#define NUM_SAMPLES_PER_PIXEL 128
#define NUM_BOUNDCE_PER_RAY 4
#define NUM_THREADS 8

#include <thread>
#include "stb/stb_image.h"
#include "stb/stbi_image_write.h"

#include "core/elixir.h"
#include "core/scene.h"
#include "camera/camera.h"
#include "material/lambertian.h"
#include "material/diffuselight.h"
#include "shape/sphere.h"
#include "shape/box.h"

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
    Interaction hit;

    if (scene.RaytraceScene(viewRay, EXR_EPSILON, viewRay.m_Distance, hit))
    {
        Ray scatteredRay;
        exrVector3 attenuation;
        exrVector3 emission = hit.m_Material->Emit(viewRay, hit);

        if (depth > 0 && hit.m_Material->Scatter(viewRay, hit, attenuation, scatteredRay))
        {
            // recursively collect color contribution 
            return emission + ShadePixel(scatteredRay, scene, depth - 1) * attenuation;
        }
        else
        {
            // If scattered ray did not intersect with any object, 
            return emission;
        }
    }
    else
    {
        //return exrVector3(0.01f);
        return SkyGradient(viewRay);
    }
}

std::unique_ptr<Scene> GenerateScene()
{
    exrProfile("Generating Scene")

    std::unique_ptr<Scene> scene = std::make_unique<Scene>();

    // Lights
    scene->AddShape(std::make_unique<Sphere>(exrPoint3(3.0f, 5.75f, -2.80f), 0.5f, std::make_unique<DiffuseLight>(exrVector3(0, 0.8f, 0.9f))));
    scene->AddShape(std::make_unique<Sphere>(exrPoint3(1.0f, 5.75f, -2.80f), 0.3f, std::make_unique<DiffuseLight>(exrVector3(0, 0.8f, 0.5f))));
    scene->AddShape(std::make_unique<Sphere>(exrPoint3(-1.0f, 5.75f, -2.80f), 0.1f, std::make_unique<DiffuseLight>(exrVector3(0.9f, 0.9f, 0.5f))));
    scene->AddShape(std::make_unique<Sphere>(exrPoint3(-3.0f, 5.75f, -2.80f), 0.05f, std::make_unique<DiffuseLight>(exrVector3(0.9f, 0.9f, 0.9f))));

    // back
    //scene->AddShape(std::make_unique<Quad>(exrPoint3(0.0f, 3.5f, -3.5f), exrVector2(7.5f, 1.5f), exrVector3(exrDegToRad(-35), 0.0f, 0.0f), std::make_unique<Metallic>(exrVector3(1.0f), 0.0f)));
    //scene->AddShape(std::make_unique<Quad>(exrPoint3(0.0f, 2.3f, -2.30f), exrVector2(7.5f, 1.5f), exrVector3(exrDegToRad(-50), 0.0f, 0.0f), std::make_unique<Metallic>(exrVector3(1.0f), 0.05f)));
    //scene->AddShape(std::make_unique<Quad>(exrPoint3(0.0f, 1.4f, -0.8f), exrVector2(7.5f, 1.5f), exrVector3(exrDegToRad(-62), 0.0f, 0.0f), std::make_unique<Metallic>(exrVector3(1.0f), 0.1f)));
    //scene->AddShape(std::make_unique<Quad>(exrPoint3(0.0f, 0.8f, 1.1f), exrVector2(7.5f, 1.5f), exrVector3(exrDegToRad(-70), 0.0f, 0.0f), std::make_unique<Metallic>(exrVector3(1.0f), 0.15f)));

    scene->AddShape(std::make_unique<Quad>(exrPoint3(0.0f, 0.0f, 0.0f), exrVector2(10000.0f), exrVector3(exrDegToRad(-90), 0, 0), std::make_unique<Lambertian>(exrVector3(1.0f))));

    exrEndProfile()
    exrInfoLine("Scene initialized with " << scene->GetSceneSize() << " shapes")

    scene->InitAccelerator();
    return scene;
}

std::unique_ptr<Scene> StandardCornellBoxScene()
{
    exrProfile("Generating Importance Sampling Scene")

    std::unique_ptr<Scene> scene = std::make_unique<Scene>();

    // light
    scene->AddEmissiveShape(std::make_unique<Quad>(exrPoint3(0.0f, 5.5f, 0.0f), exrVector2(1.3f, 1.0f), exrVector3(exrDegToRad(90), 0, 0), std::make_unique<DiffuseLight>(exrVector3(1.0f, 0.77f, 0.4f) * 7.0f)));
    // left
    scene->AddShape(std::make_unique<Quad>(exrPoint3(-2.75f, 2.75f, 0.0f), exrVector2(5.6f, 5.5f), exrVector3(0, exrDegToRad(90), 0), std::make_unique<Lambertian>(exrVector3(1.0f, 0.0f, 0.0f))));
    // right
    scene->AddShape(std::make_unique<Quad>(exrPoint3(2.75f, 2.75f, 0.0f), exrVector2(5.6f, 5.5f), exrVector3(0, exrDegToRad(-90.0f), 0), std::make_unique<Lambertian>(exrVector3(0.0f, 1.0f, 0.0f))));
    // back
    scene->AddShape(std::make_unique<Quad>(exrPoint3(0.0f, 2.75f, -2.80f), exrVector2(5.5f), exrVector3(0.0f), std::make_unique<Lambertian>(exrVector3(1.0f))));
    // floor
    scene->AddShape(std::make_unique<Quad>(exrPoint3(0.0f, 0.0f, 0.0f), exrVector2(5.5f, 5.6f), exrVector3(exrDegToRad(-90), 0, 0), std::make_unique<Lambertian>(exrVector3(1.0f))));
    // ceiling
    scene->AddShape(std::make_unique<Quad>(exrPoint3(0.0f, 5.5f, 0.0f), exrVector2(5.5f, 5.6f), exrVector3(exrDegToRad(90), 0, 0), std::make_unique<Lambertian>(exrVector3(1.0f))));
   
    scene->AddShape(std::make_unique<Box>(exrPoint3(-0.9f, 1.8f, -1.0f), exrVector3(1.6f, 3.6f, 1.6f), exrVector3(0, exrDegToRad(110), 0), std::make_unique<Lambertian>(exrVector3(1.0f, 1.0f, 1.0f))));
    scene->AddShape(std::make_unique<Box>(exrPoint3(0.9f, 0.8f, 1.0f), exrVector3(1.6f, 1.6f, 1.6f), exrVector3(0, exrDegToRad(-20), 0), std::make_unique<Lambertian>(exrVector3(1.0f, 1.0f, 1.0f))));

    exrEndProfile()
    exrInfoLine("Scene initialized with " << scene->GetSceneSize() << " shapes")

    scene->InitAccelerator();
    return scene;
}

std::unique_ptr<Scene> EmptySphere()
{
    exrProfile("Sphere Scene")
    std::unique_ptr<Scene> scene = std::make_unique<Scene>();
    scene->AddShape(std::make_unique<Sphere>(exrPoint3(0.0f, 2.5f, 0.0f), 1.0f, std::make_unique<Lambertian>(exrVector3(1.0f))));
    scene->AddShape(std::make_unique<Quad>(exrPoint3(0.0f, 1.5f, 0.0f), exrVector2(5.5f, 5.6f), exrVector3(exrDegToRad(-90), 0, 0), std::make_unique<Lambertian>(exrVector3(1.0f))));

    exrEndProfile()

    scene->InitAccelerator();
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
    std::unique_ptr<Scene> scene = EmptySphere();

    exrProfile("Raytracing Scene")

    // camera
    exrPoint3 position(0.0f, 2.75f, 10.0f);
    exrPoint3 lookat(0.0f, 2.75f, 0.0f);
    exrFloat fov = 40.0f;
    exrFloat aspect = exrFloat(OUTPUT_WIDTH) / exrFloat(OUTPUT_HEIGHT);
    exrFloat focusDist = (position - lookat).Magnitude();
    exrFloat aperture = 0.05f;
    Camera camera(position, lookat, exrVector3::Up(), fov, aspect, aperture, focusDist);

    exrU64 lastTime = Timer::TimeSinceEpochMillisec();
    //exrU64 avgTimePerRow;

    auto shade_pixel = [&](int x, int y){
        exrVector3 color;
        // Get samples
        for (int i = 0; i < NUM_SAMPLES_PER_PIXEL; i++)
        {
            exrFloat u = exrFloat(x + Random::Uniform01()) / exrFloat(OUTPUT_WIDTH);
            exrFloat v = exrFloat(y + Random::Uniform01()) / exrFloat(OUTPUT_HEIGHT);
            Ray viewRay = camera.GetViewRay(u, v);

            color += ShadePixel(viewRay, *scene, NUM_BOUNDCE_PER_RAY);
        }

        color /= NUM_SAMPLES_PER_PIXEL;

        // Clamp color values for ppm
        color.x = exrSaturate(color.x);
        color.y = exrSaturate(color.y);
        color.z = exrSaturate(color.z);

        // Correct gamma
        color = exrVector3(sqrt(color.x), sqrt(color.y), sqrt(color.z));

        // PPM takes values from 0-255;
        color *= 255.0f;

        unsigned char r = static_cast<unsigned char>(color.r);
        unsigned char g = static_cast<unsigned char>(color.g);
        unsigned char b = static_cast<unsigned char>(color.b);

        const auto offset = 3 * ((OUTPUT_HEIGHT - 1 - y)* OUTPUT_WIDTH + x) + data_offset;
        buffer[offset] = r;
        buffer[offset + 1] = g;
        buffer[offset + 2] = b;
    };

    auto rendering_trunk = [&](int tid){
        const unsigned TRUNK_SIZE = (OUTPUT_HEIGHT + NUM_THREADS - 1) / NUM_THREADS;
        const unsigned trunk_end = OUTPUT_HEIGHT < (tid + 1) * TRUNK_SIZE ? OUTPUT_HEIGHT : (tid + 1) * TRUNK_SIZE;
        for( unsigned y = tid * TRUNK_SIZE ; y < trunk_end ; ++y )
            for (int x = 0; x < OUTPUT_WIDTH; x++)
                shade_pixel(x, y);
    };

    std::thread rendering_threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i)
        rendering_threads[i] = std::thread(rendering_trunk, i);
    
    for (int i = 0; i < NUM_THREADS; ++i)
        rendering_threads[i].join();

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
    stbi_write_png(fileName.c_str(), OUTPUT_WIDTH, OUTPUT_HEIGHT, 3, output, OUTPUT_WIDTH * 3);

#ifdef EXR_PLATFORM_WIN
    system(fileName.c_str());
    system("PAUSE");
#endif

    return 0;
}
