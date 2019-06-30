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

#include "interaction.h"

exrBEGIN_NAMESPACE

class Primitive;
class BSDF;

class SurfaceInteraction : public Interaction
{
public:
    SurfaceInteraction() {};
    SurfaceInteraction(const exrPoint3& point, const exrVector3& normal, const exrVector3& wo, const Primitive* primitive)
        : Interaction(point, normal, wo)
        , m_Primitive(primitive)
        , m_BSDF(nullptr) {};

    exrVector3 GetEmission(const exrVector3& wo) const;

public:
    //! The BRDF of the surface
    BSDF* m_BSDF = nullptr;

    //! A reference to the primitive that the interaction lies on
    const Primitive* m_Primitive = nullptr;
};

exrEND_NAMESPACE
