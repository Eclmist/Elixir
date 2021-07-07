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

#include "core/elixir.h"
#include "core/bsdf/bxdf.h"

exrBEGIN_NAMESPACE

static constexpr exrU32 MaxBxDFs = 8;

class BSDF
{
public:
    BSDF(const SurfaceInteraction& si);

    void AddComponent(BxDF* b);
    exrU32 GetNumComponents(BxDF::BxDFType flags = BxDF::BxDFType::BXDFTYPE_ALL) const;
    exrSpectrum f(const exrVector3& worldWo, const exrVector3& worldWi, BxDF::BxDFType flags = BxDF::BxDFType::BXDFTYPE_ALL) const;
    exrSpectrum Sample_f(const exrVector3& worldWo, exrVector3* worldWi, exrFloat* pdf, BxDF::BxDFType flags);

    inline exrVector3 WorldToLocal(const exrVector3& v) const;
    inline exrVector3 LocalToWorld(const exrVector3& v) const;

private:
    BxDF* GetRandomBxDF(BxDF::BxDFType type = BxDF::BxDFType::BXDFTYPE_ALL);

    const exrVector3 m_Normal;
    const exrVector3 m_Tangent;
    const exrVector3 m_Bitangent;

    exrU32 m_NumBxDF = 0;
    BxDF* m_BxDFs[MaxBxDFs];
};

// Utility functions for when angles are inside the BSDF orthonormal basis
inline exrFloat CosTheta(const exrVector3& v)
{ 
    return v.z;
};

inline exrFloat Cos2Theta(const exrVector3& v) 
{ 
    return v.z * v.z;
};

inline exrFloat AbsCosTheta(const exrVector3& v)
{ 
    return abs(v.z);
};

inline exrFloat Sin2Theta(const exrVector3& v) 
{
    return exrMax(0.0f, 1.0f - Cos2Theta(v));
};

inline exrFloat SinTheta(const exrVector3& v) 
{ 
    return sqrt(Sin2Theta(v));
};

inline exrFloat TanTheta(const exrVector3& v) 
{ 
    return SinTheta(v) / CosTheta(v);
}

inline exrFloat Tan2Theta(const exrVector3& v)
{
    return Sin2Theta(v) / Cos2Theta(v);
}

inline exrFloat CosPhi(const exrVector3& v) 
{ 
    exrFloat sinTheta = SinTheta(v);
    return (sinTheta == 0) ? 1 : exrClamp(v.x / sinTheta, -1, 1);
}

inline exrFloat SinPhi(const exrVector3& v)
{
    exrFloat sinTheta = SinTheta(v); 
    return (sinTheta == 0) ? 0 : exrClamp(v.x / sinTheta, -1, 1);
}

exrEND_NAMESPACE