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

//! @brief A class that defines a sphere shape.
//! 
//! A class that defines a sphere shape and handles ray-sphere interactions
class Sphere: public Shape
{
public:
    //! @brief Constructs a sphere with a center, scale and material
    //! @param center           The origin of the sphere in world space
    //! @param radius           The radius of the sphere
    //! @param material         The material of the sphere
    Sphere(const exrPoint3& center, exrFloat radius, std::unique_ptr<Material> material)
        : Shape(material)
        , m_Radius(radius)
    {
        m_Transform.Translate(exrVector3(center.x, center.y, center.z));
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
    //! @param interaction      Output struct that contains the interaction information
    //! 
    //! @return                 True if the there is an intersection
    virtual exrBool Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax, Interaction& interaction) const override;

    //! @brief Computes a bounding volume
    //! 
    //! Computes the a bounding volume that encapsulates the current sphere.
    //! 
    //! @return                 Always return true since bounding volumes can be created for spheres
    virtual exrBool ComputeBoundingVolume() override;

private:
    //! The radius of the sphere
    exrFloat m_Radius;
};

exrEND_NAMESPACE