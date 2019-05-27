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

#include "quad.h"

exrBEGIN_NAMESPACE

bool Quad::Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax, PrimitiveHitInfo& hit) const
{
    Ray transformedRay = m_Transform.GetInverseMatrix() * ray;

    exrFloat t = (m_LocalMin.z - transformedRay.m_Origin.z) / transformedRay.m_Direction.z;
    if (t < tMin || t > tMax)
        return false;

    float x = transformedRay(t).x;
    float y = transformedRay(t).y;

    if (x < m_LocalMin.x || x > m_LocalMax.x || y < m_LocalMin.y || y > m_LocalMax.y)
        return false;

    hit.m_T = t;
    hit.m_Point = m_Transform.GetMatrix() * transformedRay(t);
    hit.m_Normal = m_Transform.GetInverseMatrix().Transposed() * exrVector3::Forward();
    hit.m_Material = m_Material.get();
    return true;
}

bool Quad::ComputeBoundingVolume()
{
    // hack for now, no scaling the bv
    exrPoint min = m_Transform.GetMatrix() * exrPoint(-0.5f, -0.5f, -EXR_EPSILON);
    exrPoint max = m_Transform.GetMatrix() * exrPoint(0.5f, 0.5f, EXR_EPSILON);

    m_BoundingVolume = BoundingVolume(min, max);
    return true;
}

exrEND_NAMESPACE