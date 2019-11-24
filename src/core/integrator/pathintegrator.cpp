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

#include "pathintegrator.h"
#include "core/bsdf/bsdf.h"
#include "core/interaction/surfaceinteraction.h"
#include "core/scene/scene.h"
exrBEGIN_NAMESPACE

exrSpectrum PathIntegrator::Li(const Ray& r, const Scene& scene, MemoryArena& arena, exrU32 depth) const
{
    exrSpectrum Lo(0.0f);
    exrSpectrum beta(1.0f); // path throughput weight, the product of the BSDF values and cosine terms so far
    Ray ray(r); // Copies the original ray, we will be updating this value at every bounce.

    for (exrU32 bounces = 0; bounces < depth ; ++bounces)
    {
        SurfaceInteraction hitRec;

        if (!scene.Intersect(ray, &hitRec)) {
            Lo += scene.SampleSkyLight(ray);
            break;
        }

        // TODO: Handle emission here

        hitRec.ComputeScatteringFunctions(ray, arena);

        // Surface hit a surface without BSDF
        if (hitRec.m_BSDF == nullptr)
        {
            exrWarningLine("Intersected a surface that has an uninitialized BSDF! Was this intended?");
            ray = hitRec.SpawnRay(ray.m_Direction);
            bounces--;
            continue;
        }

        // Sample light sources
        Lo += beta * UniformSampleOneLight(hitRec, scene, arena);

        // Sample BSDF to get new path direction
        exrVector3 wi;
        exrFloat pdf;
        exrSpectrum f = hitRec.m_BSDF->Sample_f(hitRec.m_Wo, &wi, &pdf, BxDF::BXDFTYPE_ALL);

        if (f.IsBlack() || pdf == 0.0f)
            break;

        beta *= f * AbsDot(wi, hitRec.m_Normal) / pdf;
        ray = hitRec.SpawnRay(wi);

        // Terminate path using Russian roulette
        if (bounces > 3)
        {
            exrFloat q = exrMax(0.05f, 1 - beta.GetLuminance());
            if (Random::UniformFloat() <= q)
                break;

            beta /= 1 - q;
        }
    }

    return Lo;
}

exrEND_NAMESPACE