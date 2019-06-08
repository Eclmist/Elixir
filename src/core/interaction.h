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

exrBEGIN_NAMESPACE

class BSDF;
class Primitive;

//! A struct that contains information about the local differential geometry
//! at intersection points.
struct Interaction
{
    //! Default Constructor
    Interaction() {};

    //! Constructor
    Interaction(const exrPoint3& point, const exrVector3& normal, const exrVector3& wo)
        : m_Point(point)
        , m_Normal(normal)
        , m_Wo(wo) {};

    //! The point of intersection
    exrPoint3 m_Point;

    //! The outgoing light direction Wo
    exrVector3 m_Wo;

    //! The normal of the surface at the point of intersection
    exrVector3 m_Normal;
};

class SurfaceInteraction : public Interaction
{
public:
    SurfaceInteraction() {};
    SurfaceInteraction(const exrPoint3& point, const exrVector3& normal, const exrPoint2& uv, const exrVector3& wo, const Primitive* primitive)
        : Interaction(point, normal, wo)
        , m_Primitive(primitive)
        , m_UV(uv)
        , m_BSDF() {};

    exrVector3 GetEmission(const exrVector3& wo) const;

public:
    //! A reference to the primitive that the interaction lies on
    const Primitive* m_Primitive = nullptr;
    
    //! The BSDF of the surface
    const BSDF* m_BSDF = nullptr;

    //! The UV coordinate of the shape at the point of intersection
    exrPoint2 m_UV;
};

exrEND_NAMESPACE
