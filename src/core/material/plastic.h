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

#include "material.h"
#include "core/bsdf/lambert.h"
#include "core/bsdf/mirror.h"
#include "core/bsdf/bsdf.h"

exrBEGIN_NAMESPACE

class Plastic : public Material
{
public:
    Plastic(const exrSpectrum& albedo, const exrSpectrum& specular)
        : m_Albedo(albedo)
        , m_Specular(specular) {};

    void ComputeScatteringFunctions(SurfaceInteraction* si, MemoryArena& arena) const override
    {
        si->m_BSDF = EXR_ARENA_ALLOC(arena, BSDF)(*si);
        si->m_BSDF->AddComponent(EXR_ARENA_ALLOC(arena, Lambert)(m_Albedo));
        si->m_BSDF->AddComponent(EXR_ARENA_ALLOC(arena, Mirror)(m_Specular));
    }

private:
    exrSpectrum m_Albedo;
    exrSpectrum m_Specular;
};

exrEND_NAMESPACE