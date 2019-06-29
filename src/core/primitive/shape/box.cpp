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

Box::Box(const Transform& transform, const exrVector3& scale)
    : Shape(transform, ComputeBoundingVolume())
{
    // TODO: Implement child quads

    exrVector3 halfExtent = scale / 2;
    Matrix4x4 transformMat = transform.GetMatrix();
    m_LocalCorners[0] = transformMat * exrPoint3(-halfExtent.x, -halfExtent.y, -halfExtent.z);
    m_LocalCorners[1] = transformMat * exrPoint3(halfExtent.x, -halfExtent.y, -halfExtent.z);
    m_LocalCorners[2] = transformMat * exrPoint3(-halfExtent.x, halfExtent.y, -halfExtent.z);
    m_LocalCorners[3] = transformMat * exrPoint3(halfExtent.x, halfExtent.y, -halfExtent.z);
    m_LocalCorners[4] = transformMat * exrPoint3(-halfExtent.x, -halfExtent.y, halfExtent.z);
    m_LocalCorners[5] = transformMat * exrPoint3(halfExtent.x, -halfExtent.y, halfExtent.z);
    m_LocalCorners[6] = transformMat * exrPoint3(-halfExtent.x, halfExtent.y, halfExtent.z);
    m_LocalCorners[7] = transformMat * exrPoint3(halfExtent.x, halfExtent.y, halfExtent.z);
}

exrBool Box::Intersect(const Ray& ray, exrFloat& tHit, SurfaceInteraction* interaction) const
{
    exrBool hasIntersect = false;
    Ray localRay = m_Transform.GetInverseMatrix() * ray;

    for (int i = 0; i < 6; ++i)
    {
        if (m_Sides[i]->Intersect(localRay, tHit, interaction))
        {
            interaction->m_Normal = m_Normals[i];
            interaction->m_Point = m_Transform.GetMatrix() * interaction->m_Point;
            hasIntersect = true;
        }
    }
    
    return hasIntersect;
}

AABB Box::ComputeBoundingVolume() const
{
    exrPoint3 realMin(MaxFloat);
    exrPoint3 realMax(MinFloat);

    for (int i = 0; i < 8; ++i)
    {
        realMin = Min(realMin, m_LocalCorners[i]);
        realMax = Max(realMax, m_LocalCorners[i]);
    }

    return AABB(realMin, realMax);
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
