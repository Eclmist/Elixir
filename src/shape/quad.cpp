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

bool Quad::Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax, Interaction& hit) const
{
    Ray localRay = m_Transform.GetInverseMatrix() * ray;

    exrFloat t = (-localRay.m_Origin.z) / localRay.m_Direction.z;
    if (t < tMin || t > tMax)
        return false;

    float x = localRay(t).x;
    float y = localRay(t).y;

    if (x < m_LocalMin.x || x > m_LocalMax.x || y < m_LocalMin.y || y > m_LocalMax.y)
        return false;

    hit.m_Time = t;
    hit.m_Point = m_Transform.GetMatrix() * localRay(t);
    hit.m_Normal = m_Transform.GetMatrix() * exrVector3::Forward();
    hit.m_Material = m_Material.get();
    return true;
}

bool Quad::ComputeBoundingVolume()
{
    exrPoint globalMin = m_Transform.GetMatrix() * m_LocalMin;
    exrPoint globalMax = m_Transform.GetMatrix() * m_LocalMax;

    // Swap ensure min is min and max is actually max, since we may have rotated the points above
    exrPoint realMin = exrPoint(exrMin(globalMin.x, globalMax.x), exrMin(globalMin.y, globalMax.y), exrMin(globalMin.z, globalMax.z));
    exrPoint realMax = exrPoint(exrMax(globalMin.x, globalMax.x), exrMax(globalMin.y, globalMax.y), exrMax(globalMin.z, globalMax.z));

    m_BoundingVolume = AABB(realMin, realMax);
    return true;
}

exrEND_NAMESPACE