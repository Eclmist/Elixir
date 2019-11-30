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

#include "core/elixir.h"
#include "core/bsdf/bxdf.h"

exrBEGIN_NAMESPACE

static constexpr exrU32 MaxBxDFs = 8;

class BSDF
{
public:
    BSDF(const SurfaceInteraction& si, exrFloat ior = 1.0f);

    void AddComponent(BxDF* b);
    exrU32 GetNumComponents(BxDF::BxDFType flags = BxDF::BxDFType::BXDFTYPE_ALL) const;
    exrSpectrum f(const exrVector3& worldWo, const exrVector3& worldWi, BxDF::BxDFType flags = BxDF::BxDFType::BXDFTYPE_ALL) const;
    exrSpectrum Sample_f(const exrVector3& worldWo, exrVector3* worldWi, exrFloat* pdf, BxDF::BxDFType flags);

    exrVector3 WorldToLocal(const exrVector3& v) const;
    exrVector3 LocalToWorld(const exrVector3& v) const;

public:
    const exrFloat m_RefractiveIndex;

private:
    BxDF* GetRandomBxDF(BxDF::BxDFType type = BxDF::BxDFType::BXDFTYPE_ALL);

    const exrVector3 m_Normal;
    const exrVector3 m_Tangent;
    const exrVector3 m_Bitangent;

    exrU32 m_NumBxDF = 0;
    BxDF* m_BxDFs[MaxBxDFs];
};

exrEND_NAMESPACE