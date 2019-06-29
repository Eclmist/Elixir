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

#include "system/system.h"
#include "math/math.h"
#include "math/conversionutils.h"

exrBEGIN_NAMESPACE

//! @brief A class that defines a mathematical ray
//!
//! Defines a ray with a origin, direction and distance in the parametric form of 
//! p(t) = origin + t * distance
class Ray
{
public:
    Ray() : m_TMax(MaxFloat) {};

    //! @brief Constructs a ray with an origin, direction and distance
    //! @param origin           The origin of the ray in world space
    //! @param direction        The normalized direction of the ray
    //! @param tmax             The maximum ray distance
    Ray(const exrPoint3& origin, const exrVector3& direction, exrFloat tmax = MaxFloat)
        : m_Origin(origin)
        , m_Direction(direction.Normalized())
        , m_TMax(tmax) {};

    //! @brief Copy constructor. Constructs a ray with the same origin, direction and distance from input
    //! @param copy             The ray to copy
    Ray(const Ray& copy);
    
    //! @brief Returns the point along the ray at distance t
    //! 
    //! Evaluates the ray with t and return the point on the ray such that
    //! point = origin + t * direction
    //! 
    //! @param t                The t param in parametric ray equation
    //! 
    //! @return                 The point along the ray at distance t
    inline exrPoint3 operator()(exrFloat t) const;

public:
    //! The origin of the ray in world space
    exrPoint3 m_Origin;

    //! The normalized direction of the ray
    exrVector3 m_Direction;

    //! The maximum distance of the ray
    mutable exrFloat m_TMax;
};

inline Ray operator*(const Ray& r, const Matrix4x4& m)
{
    return Ray(r.m_Origin * m, r.m_Direction * m, r.m_TMax);
}

inline Ray operator*(const Matrix4x4& m, const Ray& r)
{
    return r * m;
}

exrEND_NAMESPACE