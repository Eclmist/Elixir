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
    //! @param position         The origin of the box in world space
    //! @param scale            The scale of the box
    //! @param rotation         The rotation of the box
    //! @param material         The material of the box
    Box(const exrPoint3& position, const exrVector3& scale, const exrVector3& rotation, std::unique_ptr<Material> material)
        : Shape(material)
    {
        m_Transform.SetTranslation(exrVector3(position.x, position.y, position.z));
        m_Transform.SetRotation(rotation);

        // front
        m_Sides[0] = std::make_unique<Quad>(
            exrPoint3(0, 0, scale.z * 0.5f), 
            exrVector2(scale.x, scale.y), 
            0.0f, 
            std::make_unique<Material>());
        m_Normals[0] = m_Transform.GetMatrix() * exrVector3::Forward();

        // back
        m_Sides[1] = std::make_unique<Quad>(
            exrPoint3(0, 0, -scale.z * 0.5f), 
            exrVector2(scale.x, scale.y), 
            exrVector3(0, exrDegToRad(180), 0), 
            std::make_unique<Material>());
        m_Normals[1] = m_Transform.GetMatrix() * -exrVector3::Forward();

        // left
        m_Sides[2] = std::make_unique<Quad>(
            exrPoint3(-scale.x * 0.5f, 0, 0), 
            exrVector2(scale.z, scale.y), 
            exrVector3(0, exrDegToRad(90), 0), 
            std::make_unique<Material>());
        m_Normals[2] = m_Transform.GetMatrix() * -exrVector3::Right();

        // right
        m_Sides[3] = std::make_unique<Quad>(
            exrPoint3(+ scale.x * 0.5f, 0, 0), 
            exrVector2(scale.z, scale.y), 
            exrVector3(0, exrDegToRad(-90), 0), 
            std::make_unique<Material>());
        m_Normals[3] = m_Transform.GetMatrix() * exrVector3::Right();

        // top
        m_Sides[4] = std::make_unique<Quad>(
            exrPoint3(0, scale.y * 0.5f, 0), 
            exrVector2(scale.x, scale.z), 
            exrVector3(exrDegToRad(90), 0, 0), 
            std::make_unique<Material>());
        m_Normals[4] = m_Transform.GetMatrix() * exrVector3::Up();

        // bottom
        m_Sides[5] = std::make_unique<Quad>(
            exrPoint3(0, -scale.y * 0.5f, 0), 
            exrVector2(scale.x, scale.z), 
            exrVector3(exrDegToRad(-90), 0, 0), 
            std::make_unique<Material>());
        m_Normals[5] = m_Transform.GetMatrix() * -exrVector3::Up();


        exrVector3 halfExtent = scale / 2;
        m_LocalCorners[0] = m_Transform.GetMatrix() * exrPoint3(-halfExtent.x, -halfExtent.y, -halfExtent.z);
        m_LocalCorners[1] = m_Transform.GetMatrix() * exrPoint3(halfExtent.x, -halfExtent.y, -halfExtent.z);
        m_LocalCorners[2] = m_Transform.GetMatrix() * exrPoint3(-halfExtent.x, halfExtent.y, -halfExtent.z);
        m_LocalCorners[3] = m_Transform.GetMatrix() * exrPoint3(halfExtent.x, halfExtent.y, -halfExtent.z);
        m_LocalCorners[4] = m_Transform.GetMatrix() * exrPoint3(-halfExtent.x, -halfExtent.y, halfExtent.z);
        m_LocalCorners[5] = m_Transform.GetMatrix() * exrPoint3(halfExtent.x, -halfExtent.y, halfExtent.z);
        m_LocalCorners[6] = m_Transform.GetMatrix() * exrPoint3(-halfExtent.x, halfExtent.y, halfExtent.z);
        m_LocalCorners[7] = m_Transform.GetMatrix() * exrPoint3(halfExtent.x, halfExtent.y, halfExtent.z);

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
    //! Sides of the boxes, internally handled as quads
    std::unique_ptr<Quad> m_Sides[6];

    //! Normal vectors that corresponds with the side quads
    exrVector3 m_Normals[6];    

    //! Local space min point for bounding volume
    exrPoint3 m_LocalCorners[8];
};

exrEND_NAMESPACE