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

void BSDF::AddComponent(const BxDF* bxdf)
{
    exrAssert(m_NumBxDF < MaxBxDFs, "Max number of BxDFs exceeded for material!");
    m_BxDFs[m_NumBxDF++] = bxdf;
}

exrU32 BSDF::GetComponentCount(BxDF::BxDFType flags) const
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
        if (m_BxDFs[i]->MatchesFlags(flags) && (reflect && m_BxDFs[i]->MatchesFlags(BxDF::BSDF_REFLECTION)) &&
            (!reflect && m_BxDFs[i]->MatchesFlags(BxDF::BSDF_TRANSMISSION)))
        {
            res += m_BxDFs[i]->Evaluate(wo, wi);
        }
    }

    return res;
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


exrEND_NAMESPACE