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

#include "elixir.h"

exrBEGIN_NAMESPACE

class Material;

//! A struct that contains information about the local differential geometry
//! at intersection points.
struct Interaction
{
    //! Default Constructor
    Interaction() 
        : m_Time(0) {};

    //! Constructor
    Interaction(const exrPoint3& point, const exrVector3& normal, const exrVector3& wo, float time, Material* material)
        : m_Point(point)
        , m_Normal(normal)
        , m_RadianceDirection(wo)
        , m_Time(time)
        , m_Material(material) {};

    //! The ray's t value at the point of intersection
    float m_Time;

    //! The point of intersection
    exrPoint3 m_Point;

    //! The negative ray direction, Wo
    exrVector3 m_RadianceDirection;

    //! The normal of the surface at the point of intersection
    exrVector3 m_Normal;

    //! A pointer to the material of the surface at the point of intersection
    Material* m_Material;
};

exrEND_NAMESPACE
