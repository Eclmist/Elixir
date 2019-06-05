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

#include "box.h"

exrBEGIN_NAMESPACE

Box::Box(const exrPoint3& position, const exrVector3& scale, const exrVector3& rotation, std::unique_ptr<Material> material)
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
}

bool Box::Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax, Interaction& interaction) const
{
    exrBool hasIntersect = false;
    Ray localRay = m_Transform.GetInverseMatrix() * ray;

    for (exrU32 i = 0; i < 6; i++)
    {
        if (m_Sides[i]->Intersect(localRay, tMin, tMax, interaction))
        {
            tMax = interaction.m_Time;
            interaction.m_Normal = m_Normals[i];
            interaction.m_Point = m_Transform.GetMatrix() * localRay(interaction.m_Time);
            interaction.m_Material = m_Material.get();
            hasIntersect = true;
        }
    }
    
    return hasIntersect;
}

bool Box::ComputeBoundingVolume()
{
    exrPoint3 realMin(EXR_MAX_FLOAT);
    exrPoint3 realMax(EXR_MIN_FLOAT);

    for (exrU32 i = 0; i < 8; i++)
    {
        realMin = exrPoint3(exrMin(realMin.x, m_LocalCorners[i].x), exrMin(realMin.y, m_LocalCorners[i].y), exrMin(realMin.z, m_LocalCorners[i].z));
        realMax = exrPoint3(exrMax(realMax.x, m_LocalCorners[i].x), exrMax(realMax.y, m_LocalCorners[i].y), exrMax(realMax.z, m_LocalCorners[i].z));
    }

    m_BoundingVolume = AABB(realMin, realMax);

    return true;
}

exrFloat Box::GetArea() const
{
    throw std::logic_error("The method or operation is not implemented.");
}

Interaction Box::Sample(const exrPoint2& u) const
{
    throw std::logic_error("The method or operation is not implemented.");
}

exrEND_NAMESPACE
