/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019-2020 Samuel Van Allen - All rights reserved.

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

BSDF::BSDF(const SurfaceInteraction& si)
    : m_Normal(si.m_Normal)
    , m_Tangent(Cross(m_Normal, UniformSampleSphere().Normalized()))
    , m_Bitangent(Cross(m_Normal, m_Tangent).Normalized())
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

exrSpectrum BSDF::f(const exrVector3& worldWo, const exrVector3& worldWi, BxDF::BxDFType flags) const
{
    exrVector3 wi = WorldToLocal(worldWi);
    exrVector3 wo = WorldToLocal(worldWo);

    exrBool reflect = ((Dot(worldWi, m_Normal) * Dot(worldWo, m_Normal)) > 0);
    exrSpectrum res;

    for (exrU32 i = 0; i < m_NumBxDF; ++i)
    {
        if (m_BxDFs[i]->MatchesFlags(flags))
        {
            if (reflect && !(m_BxDFs[i]->HasFlags(BxDF::BXDFTYPE_HAS_REFLECTANCE)))
                continue;
            
            if (!reflect && !m_BxDFs[i]->HasFlags(BxDF::BXDFTYPE_HAS_TRANSMISSION))
                continue;

            res += m_BxDFs[i]->f(wo, wi);
        }
    }

    return res;
}

exrSpectrum BSDF::Sample_f(const exrVector3& worldWo, exrVector3* worldWi, exrFloat* pdf, BxDF::BxDFType flags)
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
    sampledBxDF->Sample_f(wo, &wi, pdf);

    if (pdf == 0)
        return exrSpectrum(0.0f);

    *worldWi = LocalToWorld(wi);

    // The pdf have to be adjusted based on all the bxdf that could have been sampled
    if (numMatchingBxdfs > 1)
    {
        for (exrU32 i = 0; i < m_NumBxDF; ++i)
        {
            if (m_BxDFs[i] != sampledBxDF && m_BxDFs[i]->MatchesFlags(flags))
            {
                *pdf += m_BxDFs[i]->Pdf(wo, wi);
            }
        }

        *pdf /= numMatchingBxdfs;
    }

    // now that we have the correct wi direction, evaluate this bsdf as per normal
    return f(worldWo, *worldWi, flags);
}

exrVector3 BSDF::WorldToLocal(const exrVector3& v) const
{
    return exrVector3(Dot(v, m_Bitangent), Dot(v, m_Tangent), Dot(v, m_Normal));
}

exrVector3 BSDF::LocalToWorld(const exrVector3& v) const
{
    return exrVector3(m_Bitangent.x * v.x + m_Tangent.x * v.y + m_Normal.x * v.z,
                      m_Bitangent.y * v.x + m_Tangent.y * v.y + m_Normal.y * v.z,
                      m_Bitangent.z * v.x + m_Tangent.z * v.y + m_Normal.z * v.z);
}

BxDF* BSDF::GetRandomBxDF(BxDF::BxDFType type)
{
    exrU32 numMatching = GetNumComponents(type);

    exrU32 counter = Random::UniformUInt32(numMatching - 1);

    for (exrU32 i = 0; i < m_NumBxDF; ++i)
    {
        if (m_BxDFs[i]->MatchesFlags(type) && counter-- == 0)
            return m_BxDFs[i];
    }

    return nullptr;
}

exrEND_NAMESPACE