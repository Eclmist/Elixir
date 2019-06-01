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
    //! @param position         The origin of the box in world space
    //! @param scale            The scale of the box
    //! @param rotation         The rotation of the box
    //! @param material         The material of the box
    Quad(const exrPoint3& position, const exrVector2& scale, const exrVector3& rotation, std::unique_ptr<Material> material)
        : Shape(material)
    {
        m_Transform.SetTranslation(exrVector3(position.x, position.y, position.z));
        m_Transform.SetRotation(rotation);

        m_LocalMin = exrPoint3::Zero() - exrVector3(0.5f * scale.x, 0.5f * scale.y, EXR_EPSILON);
        m_LocalMax = exrPoint3::Zero() + exrVector3(0.5f * scale.x, 0.5f * scale.y, EXR_EPSILON);

        ComputeBoundingVolume();
    };

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
    virtual exrBool Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax, Interaction& interaction) const override;

    //! @brief Computes a bounding volume
    //! 
    //! Computes the a bounding volume that encapsulates the current box.
    //! 
    //! @return                 Always return true since bounding volumes can be created for quads
    virtual exrBool ComputeBoundingVolume() override;

public:
    exrPoint3 m_LocalMin;
    exrPoint3 m_LocalMax;
};

exrEND_NAMESPACE