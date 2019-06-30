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
    BSDF(const SurfaceInteraction& si, exrFloat ior = 1)
        : m_ReflectiveIndex(ior)
        , m_ShadingNormal(si.m_Normal)
        , m_NumBxDF(0) {};

    void AddComponent(const BxDF* b);
    exrU32 GetComponentCount(BxDF::BxDFType flags = BxDF::BxDFType::BSDF_ALL) const;

public:
    const exrFloat m_ReflectiveIndex;

private:
    const exrVector3 m_ShadingNormal;
    exrU32 m_NumBxDF;
    const BxDF* m_BxDFs[MaxBxDFs];
};

exrEND_NAMESPACE