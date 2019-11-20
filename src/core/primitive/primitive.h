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
#include "core/light/arealight.h"
#include "core/material/material.h"
#include "core/primitive/shape/shape.h"

exrBEGIN_NAMESPACE

class AABB;

//! Bridges the geometry processing and the shading subsystems
class Primitive
{
public:
    //! @brief Returns the bounding volume of the primitive
    //! @return                 The bounding volume of the primitive
    AABB GetBoundingVolume() const;

    //! @brief Test the geometry for intersections with a ray
    //! 
    //! Test the geometry for intersection with the ray, and outputs the surface
    //! intersection data in <interaction> 
    //!
    //! @param ray              The ray to test against
    //! @param interaction      The output surface interaction struct
    //! 
    //! @return                 True if the there is an intersection
    exrBool Intersect(const Ray& ray, SurfaceInteraction* interaction) const;

    //! @brief Test the geometry for intersections with a ray
    //! 
    //! Test the geometry for intersection with the ray, but ignore surface data.
    //! Useful for shadow rays that don't require surface data.
    //!
    //! @param ray              The ray to test against
    //! @param tHit             The t-value of the hitpoint along the ray
    //! 
    //! @return                 True if the there is an intersection
    exrBool HasIntersect(const Ray& r, exrFloat& tHit) const;

    //! @brief An overload for HasIntersect for when the result of tHit is not needed
    //! 
    //! @param ray              The ray to test against
    //! @return                 True if the there is an intersection
    exrBool HasIntersect(const Ray& r) const;

    //! Returns the material of the current primitive
    const Material* GetMaterial() const;

public:
    //! The underlying shape that describes the primitive
    std::unique_ptr<Shape> m_Shape;
    
    //! The material assigned to the primitive
    Material* m_Material;
};

exrEND_NAMESPACE