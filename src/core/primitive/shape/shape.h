/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019-2020 Samuel Van Allen - All rights reserved.

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
#include "core/material/material.h"
#include "core/spatial/utils/aabb.h"

exrBEGIN_NAMESPACE

class Primitive;

//! @brief A base class that all shapes should inherit from
//! 
//! A base class that all shape shapes such as triangles and spheres should inherit from.
//! Contains various pure virtual functions that should be overridden by individual shapes,
//! such as for computing its bounding volume and handling ray intersections
class Shape
{
public:
    //! @brief Test the geometry for intersections with a ray
    //! 
    //! This function allows us to do intersection tests with a segment of a ray, and
    //! outputs the interaction info into <interaction>
    //! 
    //! @param ray              The ray to test against
    //! @param tHit             The t value of ray at the point of intersection, if any
    //! @param interaction      Output struct that contains the interaction information
    //! 
    //! @return                 True if the there is an intersection
    virtual exrBool Intersect(const Ray& ray, exrFloat& tHit, SurfaceInteraction* interaction) const = 0;

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
    virtual exrBool HasIntersect(const Ray& ray, exrFloat& tHit) const = 0;

public:
    //! Computes a bounding volume that encapsulates the current geometry.
    virtual AABB ComputeBoundingVolume() const = 0;

protected:
    friend class Primitive;

    //! Reference to the primitive that is holding this shape.
    //! This is needed because we often need the transform stored in the primitive.
    Primitive* m_Primitive;
};

exrEND_NAMESPACE