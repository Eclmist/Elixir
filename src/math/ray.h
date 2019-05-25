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

//! @brief A class that defines a mathematical ray
//!
//! Defines a ray with a origin, direction and distance in the parametric form of 
//! p(t) = origin + t * distance
class Ray
{
public:
    //! @brief Constructs a ray with an origin, direction and distance
    //! @param origin           The origin of the ray in world space
    //! @param direction        The normalized direction of the ray
    //! @param distance         The maximum ray distance
    Ray(const exrPoint& origin = exrPoint::Zero(), const exrVector3& direction = exrVector3::Zero(), exrFloat distance = 1000.0f)
        : m_Origin(origin), m_Direction(direction), m_Distance(distance) {};

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
    inline exrPoint operator()(exrFloat t) const { return m_Origin + t * m_Direction; }

public:
    //! The origin of the ray in world space
    exrPoint m_Origin;

    //! The normalized direction of the ray
    exrVector3 m_Direction;

    //! The maximum distance of the ray
    exrFloat m_Distance;
};

exrEND_NAMESPACE