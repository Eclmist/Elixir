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
#include "core/aabb.h"
#include "material/material.h"
#include "math/transform.h"

exrBEGIN_NAMESPACE

//! @brief A base class that all shapes should inherit from
//! 
//! A base class that all shape shapes such as triangles and spheres should inherit from.
//! Contains various pure virtual functions that should be overridden by individual shapes,
//! such as for computing its bounding volume and handling ray intersections
class Shape
{
public:
    //! @brief Constructs a shape
    //! @param material         The material of the shape
    Shape(std::unique_ptr<Material>& material)
        : m_Material(std::move(material)) {};

public:
    //! @brief Test the geometry for intersections with a ray
    //! 
    //! This function allows us to do intersection tests with a segment of a ray in the domain
    //! of tMin and tMax
    //! 
    //! @param ray              The ray to test against
    //! @param tMin             Min t value of ray to test
    //! @param tMax             Max t value of ray to test
    //! @param interaction      Output struct that contains the interaction information
    //! 
    //! @return                 True if the there is an intersection
    virtual bool Intersect(const Ray& ray, float tMin, float tMax, Interaction& interaction) const = 0;

public:
    //! @brief Returns the bounding volume of the shape
    //! @return                 A bounding volume of the shape
    inline AABB GetBoundingVolume()
    {
        return m_BoundingVolume;
    };

protected :
    //! @brief Computes a bounding volume
    //! 
    //! Computes the a bounding volume that encapsulates the current geometry.
    //!
    //! @return                 True if the bounding volume was successfully created
    virtual bool ComputeBoundingVolume() = 0;

protected:
    //! A pointer to the material of the shape
    const std::unique_ptr<Material> m_Material;
    
    //! A bounding volume that contains the shape;
    AABB m_BoundingVolume;

    //! The transform of the shape
    Transform m_Transform;
};

exrEND_NAMESPACE