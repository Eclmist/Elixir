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

#include "shape.h"

exrBEGIN_NAMESPACE

//! @brief A class that defines a box shape.
//! 
//! A class that defines a box shape and handles ray-box interactions
class Quad : public Shape
{
public:
    //! @brief Constructs a box with a position and a scale
    Quad(const Transform& transform, const exrVector2& scale);

    //! @brief Test the box for intersections with a ray
    //! 
    //! This function allows us to do intersection tests with a segment of a ray, and
    //! outputs the interaction info into <interaction>
    //! 
    //! @param ray              The ray to test against
    //! @param tHit             The t value of ray at the point of intersection, if any
    //! @param interaction      Output struct that contains the interaction information
    //! 
    //! @return                 True if the there is an intersection
    exrBool Intersect(const Ray& ray, exrFloat& tHit, SurfaceInteraction* interaction) const override;

    //! @brief Test the geometry for intersections with a ray
    //! 
    //! This function allows us to do intersection tests with a segment of a ray, but
    //! does not initialize interaction or hit info. Useful for when checking if a ray
    //! is obstructed, such as with shadow rays
    //! 
    //! @param ray              The ray to test against
    //! @param tHit             The t value of ray at the point of intersection, if any
    //! 
    //! @return                 True if the there is an intersection
    exrBool HasIntersect(const Ray& ray, exrFloat& tHit) const override;

    //! @brief Computes a bounding volume
    //! 
    //! Computes the a bounding volume that encapsulates the current box.
    //! 
    //! @return                 Always return true since bounding volumes can be created for quads
    AABB ComputeBoundingVolume() const override;

public:
    exrPoint3 m_HalfExtents;
};

exrEND_NAMESPACE