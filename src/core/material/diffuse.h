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
#include "core/bsdf/bsdf.h"

exrBEGIN_NAMESPACE

class Diffuse : public Material
{
public:
    Diffuse(const exrSpectrum& albedo)
        : m_Albedo(albedo) {}

    void ComputeScatteringFunctions(SurfaceInteraction* si) const override
    {
        si->m_BSDF = new BSDF(*si);
        // We need to create a new bxdf for each interaction because properties such as color may change based on 
        // the material definition (textures, etc)
        si->m_BSDF->AddComponent(new Lambert(m_Albedo));
    }

private:
    exrSpectrum m_Albedo;
};

exrEND_NAMESPACE