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

#include "wittedintegrator.h"
#include "core/scene/scene.h"
#include "core/bsdf/bsdf.h"

exrBEGIN_NAMESPACE

exrSpectrum WittedIntegrator::Evaluate(const RayDifferential& ray, const Scene& scene, exrU32 depth) const
{
    exrSpectrum L(0.0);
    SurfaceInteraction interaction;
    if (!scene.Intersect(ray, &interaction))
    {
        return 0.0f;
        return scene.SampleSkyLight(ray);
    }

    exrVector3 normal = interaction.m_Normal;
    exrVector3 wo = interaction.m_Wo;
    interaction.ComputeScatteringFunctions(ray);

    // Handle emissive surfaces
    L += interaction.EvaluateEmission(wo);

    // Add contribution of each light source
    for (const auto& light : scene.m_Lights)
    {
        if (!light->IsDeltaLight())
            continue;

        exrVector3 wi;
        exrFloat pdf;
        exrPoint2 uv; // TODO: support this param
        exrSpectrum Li = light->Sample(interaction, uv, wi, pdf);

        exrAssert(Li.ToRGB().MagnitudeSquared() >= 0, "Light returning negative values somehow");

        if (Li.IsBlack() || pdf == 0)
            continue;

        exrSpectrum f = interaction.m_BSDF->Evaluate(wo, wi);
        exrAssert(f.ToRGB().MagnitudeSquared() >= 0, "BSDF returning negative values somehow");

        if (f.IsBlack())
            continue;

        if (scene.HasIntersect(Ray(interaction.m_Point, wi, (light->m_Transform.GetPosition() - interaction.m_Point).Magnitude())))
            continue;

        L += f * Li * abs(Dot(wi, normal)) / pdf;
    }

    if (depth + 1 < m_NumBouncePerPixel)
    {
        L += SpecularReflect(ray, interaction, scene, depth);
        //L += SpecularTransmit(ray, interaction, scene, depth);
    }

    return L;
}

exrEND_NAMESPACE