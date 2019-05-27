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

#include "primitive.h"

exrBEGIN_NAMESPACE

//! @brief A class that defines a sphere primitive.
//! 
//! A class that defines a sphere primitive and handles ray-sphere interactions
class Sphere: public Primitive
{
public:
    //! @brief Constructs a sphere with a center, scale and material
    //! @param center           The origin of the sphere in world space
    //! @param scale            The scale of the sphere
    //! @param material         The material of the sphere
    Sphere(const exrPoint& center, exrVector3 scale, std::unique_ptr<Material> material)
        : Primitive(material)
    {
        m_Transform.Translate(exrVector3(center.x, center.y, center.z));
        m_Transform.Scale(exrVector3(scale));
        ComputeBoundingVolume();
    };

    //! @brief Test the sphere for intersections with a ray
    //! 
    //! This function allows us to do intersection tests with a segment of a ray in the domain
    //! of tMin and tMax
    //! 
    //! @param ray              The ray to test against
    //! @param tMin             Min t value of ray to test
    //! @param tMax             Max t value of ray to test
    //! @param hitInfo          Output struct that contains the hit information
    //! 
    //! @return                 True if the there is an intersection
    virtual exrBool Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax, PrimitiveHitInfo& hitInfo) const override;

    //! @brief Computes a bounding volume
    //! 
    //! Computes the a bounding volume that encapsulates the current sphere.
    //! 
    //! @return                 Always return true since bounding volumes can be created for spheres
    virtual exrBool ComputeBoundingVolume() override;
};

exrEND_NAMESPACE