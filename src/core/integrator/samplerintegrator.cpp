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

#include "samplerintegrator.h"
#include "core/bsdf/bxdf.h"
#include "core/bsdf/bsdf.h"

exrBEGIN_NAMESPACE

void SamplerIntegrator::Render(const Scene& scene)
{
    Preprocess(scene);

    Exporter* exporter = m_Camera->m_Exporter.get();
    
    // Compute number of tiles
    const Point2<exrU32> resolution = exporter->m_Resolution;
    const Point2<exrU32> numTiles((resolution.x + TileSize - 1) / TileSize, (resolution.y + TileSize - 1) / TileSize);

    const exrU32 totalNumTiles = numTiles.x * numTiles.y;
    AtomicFloat progress;

    exrProfile("Rendering Scene");
    { // let threadPool destructor join all threads
        exrAssert(g_RuntimeOptions.numThreads > 0, "Unable to start elixir with 0 threads!");
        ThreadPool threadPool(g_RuntimeOptions.numThreads);

        // Loop in terms of x,y tiles so this can become async in the future
        for (exrU32 i = 0; i < totalNumTiles; ++i)
        {
            const exrU32 tx = i % numTiles.x;
            const exrU32 ty = exrU32(i / exrFloat(numTiles.x));
            
            const exrFloat priority = abs(exrFloat(tx - numTiles.x)) + abs(exrFloat(ty - numTiles.y));

            threadPool.ScheduleTask(priority, [&](exrU32 tileX, exrU32 tileY)
            {
                MemoryArena memoryArena;
                // Everything from this point must explicitly enforce thread safety!
                // Compute bounds for tile
                Point2<exrU32> tileMin(tileX * TileSize, tileY * TileSize);
                Point2<exrU32> tileMax(tileX * TileSize + TileSize, tileY * TileSize + TileSize);

                // Foreach pixel, shade
                for (exrU32 x = 0; x < TileSize; ++x)
                {
                    for (exrU32 y = 0; y < TileSize; ++y)
                    {
                        // Foreach sample
                        for (exrU32 n = 0; n < m_NumSamplesPerPixel; ++n)
                        {
                            exrPoint2 randomInDisc = ConcentricSampleDisk();
                            exrFloat u = exrFloat(tileMin.x + x + randomInDisc.x) / exrFloat(resolution.x);
                            exrFloat v = exrFloat(tileMin.y + y + randomInDisc.y) / exrFloat(resolution.y);
                            Ray viewRay = m_Camera->GetViewRay(u, v);

                            exrSpectrum L(0.0f);
                            L += Li(viewRay, scene, memoryArena, m_NumBouncePerPixel);

                            // Issue warnings if unexpected radiance is returned
                            if (L.HasNaNs())
                            {
                                exrError("NaNs radiance returned by integrator.");
                                L = exrSpectrum::FromRGB(exrVector3(1, 0, 1));
                            }

                            exporter->WritePixel(Point2<exrU32>(tileMin.x + x, tileMin.y + y), L);
                            memoryArena.Release();
                        }
                    }
                }

            }, tx, ty);
        }
    }
    exrEndProfile();

    exporter->WriteImage(1.0f / m_NumSamplesPerPixel);
}

exrSpectrum SamplerIntegrator::SpecularReflect(const Ray& ray, const SurfaceInteraction& intersect,
    const Scene& scene, MemoryArena& arena, exrU32 depth) const
{
    exrVector3 wo = intersect.m_Wo;
    exrVector3 wi;
    exrFloat pdf;
    BxDF::BxDFType type = BxDF::BxDFType(BxDF::BSDF_REFLECTION | BxDF::BSDF_SPECULAR);
    exrSpectrum f = intersect.m_BSDF->Sample_f(wo, &wi, &pdf, type);

    const exrVector3& normal = intersect.m_Normal;
    if (pdf > 0 && !f.IsBlack() && abs(Dot(wi, normal)) > 0)
    {
        Ray reflRay = intersect.SpawnRay(wi);
        return f * Li(reflRay, scene, arena, depth - 1) * abs(Dot(wi, normal)) / pdf;
    }
    else
    {
        return exrSpectrum(0.0f);
    }
}

exrSpectrum SamplerIntegrator::SpecularRefract(const Ray& ray, const SurfaceInteraction& intersect,
    const Scene& scene, MemoryArena& arena, exrU32 depth) const
{
    throw "Not yet implemented!";
}

exrEND_NAMESPACE
