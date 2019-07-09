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

#pragma once

#include "bsdf.h"

exrBEGIN_NAMESPACE

BSDF::BSDF(const SurfaceInteraction& si, exrFloat ior)
    : m_ReflectiveIndex(ior)
    , m_ShadingNormal(si.m_ShadingInfo.m_Normal)
    , m_GeometricNormal(si.m_Normal)
    , m_ShadingTangent(Cross(m_ShadingNormal, UniformSampleSphere(Uniform01Point2())))
    , m_ShadingBitangent(Cross(m_ShadingNormal, m_ShadingTangent))
{
}

void BSDF::AddComponent(BxDF* bxdf)
{
    exrAssert(m_NumBxDF < MaxBxDFs, "Max number of BxDFs exceeded for material!");
    m_BxDFs[m_NumBxDF++] = bxdf;
}

exrU32 BSDF::GetNumComponents(BxDF::BxDFType flags) const
{
    exrU32 res = 0;
    for (exrU32 i = 0; i < m_NumBxDF; ++i)
        if (m_BxDFs[i]->MatchesFlags(flags)) ++res;

    return res;
}

exrSpectrum BSDF::Evaluate(const exrVector3& worldWo, const exrVector3& worldWi, BxDF::BxDFType flags) const
{
    exrVector3 wi = WorldToLocal(worldWi);
    exrVector3 wo = WorldToLocal(worldWo);

    exrBool reflect = ((Dot(worldWi, m_GeometricNormal) * Dot(worldWo, m_GeometricNormal)) > 0);
    exrSpectrum res;

    for (exrU32 i = 0; i < m_NumBxDF; ++i)
    {
        if (m_BxDFs[i]->MatchesFlags(flags))
        {
            if (reflect && !m_BxDFs[i]->MatchesFlags(BxDF::BSDF_REFLECTION))
                continue;
            
            if (!reflect && !m_BxDFs[i]->MatchesFlags(BxDF::BSDF_TRANSMISSION))
                continue;

            res += m_BxDFs[i]->Evaluate(wo, wi);
        }
    }

    return res;
}

exrSpectrum BSDF::Sample(const exrVector3& worldWo, exrVector3* worldWi, exrFloat* pdf, BxDF::BxDFType flags)
{
    // We can only sample one bxdf at a time, but since we may have more than one bxdf we will randomly select
    // one that matches the criteria.

    BxDF* sampledBxDF = GetRandomBxDF(flags);

    if (sampledBxDF == nullptr)
    {
        *pdf = 0.0f;
        return exrSpectrum(0.0f);
    }

    exrU32 numMatchingBxdfs = GetNumComponents(flags);

    exrVector3 wo = WorldToLocal(worldWo);
    exrVector3 wi;
    sampledBxDF->Sample(wo, &wi, pdf, flags);

    if (pdf == 0)
        return exrSpectrum(0.0f);

    *worldWi = LocalToWorld(wi);

    // The pdf have to be adjusted based on all the bxdf that could have been sampled
    if (numMatchingBxdfs > 1)
    {
        for (BxDF* bxdf : m_BxDFs)
        {
            if (bxdf != sampledBxDF && bxdf->MatchesFlags(flags))
            {
                *pdf += bxdf->Pdf(wo, wi);
            }
        }

        *pdf /= numMatchingBxdfs;
    }

    // now that we have the correct wi direction, evaluate this bsdf as per normal
    return Evaluate(worldWo, *worldWi, flags);
}

exrVector3 BSDF::WorldToLocal(const exrVector3& v) const
{
    return exrVector3(Dot(v, m_ShadingBitangent), Dot(v, m_ShadingTangent), Dot(v, m_ShadingNormal));
}

exrVector3 BSDF::LocalToWorld(const exrVector3& v) const
{
    return exrVector3(m_ShadingBitangent.x * v.x + m_ShadingTangent.x * v.y + m_ShadingNormal.x * v.z,
                      m_ShadingBitangent.y * v.x + m_ShadingTangent.y * v.y + m_ShadingNormal.y * v.z,
                      m_ShadingBitangent.z * v.x + m_ShadingTangent.z * v.y + m_ShadingNormal.z * v.z);
}

BxDF* BSDF::GetRandomBxDF(BxDF::BxDFType type)
{
    exrU32 numMatching = GetNumComponents(type);

    exrU32 counter = exrU32(Uniform01() * numMatching);

    for (exrU32 i = 0; i < m_NumBxDF; ++i)
    {
        if (m_BxDFs[i]->MatchesFlags(type) && counter-- == 0)
            return m_BxDFs[i];
    }

    return nullptr;
}


exrEND_NAMESPACE