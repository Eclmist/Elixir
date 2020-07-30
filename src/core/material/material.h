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
#include "core/material/tslmanager.h"

exrBEGIN_NAMESPACE

//! A representation of a surface shader unique to each primitive in the scene.
//! This encompasses all the shading related properties of a *primitive*.
//! Not to be confused with the BSDF class, which will be different for every
//! shading *point*.
class Material
{
public:
    virtual void ComputeScatteringFunctions(SurfaceInteraction* si, MemoryArena& arena) const = 0;

public:
    // TODO: These are all TSL related stuff, should move them to somewhere else probably.
    std::shared_ptr<Tsl_Namespace::ShaderUnitTemplate> m_ShaderTemplate;
    std::shared_ptr<Tsl_Namespace::ShaderInstance> m_ShaderInstance;
    shader_raw_func m_ShaderFunction;

public:
    inline exrSpectrum GetAlbedo() const { return m_Albedo; };
    inline exrSpectrum GetSpecular() const { return m_Specular; };
    inline exrFloat GetRoughness() const { return m_Roughness; };

protected:
    exrSpectrum m_Albedo;
    exrSpectrum m_Specular;
    exrFloat    m_Roughness;
};

exrEND_NAMESPACE
