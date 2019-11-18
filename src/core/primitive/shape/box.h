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

#include "quad.h"

exrBEGIN_NAMESPACE

//! @brief A class that defines a box shape.
//! 
//! A class that defines a box shape and handles ray-box interactions
class Box : public Shape
{
public:
    //! @brief Constructs a box with a position and a scale
    Box(const Transform& transform, const exrVector3& scale);

    //! @brief Test the box for intersections with a ray
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
    exrBool Intersect(const Ray& ray, exrFloat& tHit, SurfaceInteraction* interaction) const override;

protected:
    //! @brief Computes a bounding volume
    //! 
    //! Computes the a bounding volume that encapsulates the current box.
    //! 
    //! @return                 Always return true since bounding volumes can be created for quads
    AABB ComputeBoundingVolume() const override;

private:
    //! Sides of the boxes, internally handled as quads
    std::unique_ptr<Quad> m_Sides[6];

    //! Normal vectors that corresponds with the side quads
    exrVector3 m_Normals[6];    

    //! Local space min point for bounding volume
    exrPoint3 m_LocalCorners[8];

};

exrEND_NAMESPACE