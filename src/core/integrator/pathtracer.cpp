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

#include "pathtracer.h"
#include "core/bsdf/bsdf.h"
#include "core/interaction/surfaceinteraction.h"
#include "core/scene/scene.h"
exrBEGIN_NAMESPACE

exrSpectrum PathTracer::Li(const Ray& ray, const Scene& scene, MemoryArena& arena, exrU32 depth) const
{
    SurfaceInteraction hitRec;
    exrSpectrum Lo(0.0f);

    if (!scene.Intersect(ray, &hitRec)) {
        return scene.SampleSkyLight(ray);
    }

    // Init BSDF
    hitRec.ComputeScatteringFunctions(ray, arena);

    // Sample light sources
    for (const Light* light : scene.m_Lights) 
    {
        exrVector3 wi;
        exrFloat pdf;
        exrSpectrum attenuation = light->Sample_f(hitRec, wi, pdf);
        exrSpectrum col = hitRec.m_BSDF->f(hitRec.m_Wo, wi) * attenuation * Dot(hitRec.m_Normal, wi);

        Ray shadowRay = hitRec.SpawnRay(wi, Distance(light->m_Transform.GetPosition(), hitRec.m_Point));
        if (scene.HasIntersect(shadowRay))
             continue;

        Lo += col;
    }

    // Spawning secondary ray
    exrVector3 wi;
    exrFloat pdf;
    BxDF::BxDFType type = BxDF::BxDFType(BxDF::BSDF_ALL);
    exrSpectrum f = hitRec.m_BSDF->Sample_f(hitRec.m_Wo, &wi, &pdf, type);

    exrFloat ndotwi = Dot(wi, hitRec.m_Normal);
    if (pdf > 0 && !f.IsBlack() && ndotwi > 0 && depth > 0)
    {
        Ray reflRay = hitRec.SpawnRay(wi);
        return Lo + f * Li(reflRay, scene, arena, depth - 1) * abs(ndotwi) / pdf;
    }

    return Lo;
}

exrEND_NAMESPACE