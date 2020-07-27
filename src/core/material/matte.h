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
#include "core/bsdf/orennayar.h"
#include "core/bsdf/bsdf.h"
#include "core/material/tslmanager.h"

exrBEGIN_NAMESPACE

class Matte : public Material
{
public:
    Matte(const exrSpectrum& albedo, exrFloat roughness)
    {
        m_Albedo = albedo;
        m_Roughness = roughness;
    }

    void ComputeScatteringFunctions(SurfaceInteraction* si, MemoryArena& arena) const override
    {
        si->m_BSDF = EXR_ARENA_ALLOC(arena, BSDF)(*si);
        // We need to create a new bxdf for each interaction because properties such as color may change based on 
        // the material definition (textures, etc)

        // This was the original way of creating and assigning a new brdf into the bsdf wrapper.
        // However, with TSL, the bsdf will be created based on output of the TSL shader, wrapped
        // by their closure object. This means that m_Albedo and m_Roughness should be passed directly into the 
        // TSL shader.

        // si->m_BSDF->AddComponent(EXR_ARENA_ALLOC(arena, OrenNayar)(m_Albedo, m_Roughness));
        si->m_BSDF->AddComponent(GetBxDF(this, *si, arena));
    }
};

exrEND_NAMESPACE