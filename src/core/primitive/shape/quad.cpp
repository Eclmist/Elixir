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

exrBool Quad::Intersect(const Ray& ray, exrFloat& tHit, SurfaceInteraction* interaction) const
{
    Ray localRay = m_Transform.GetInverseMatrix() * ray;

    exrFloat t = (-localRay.m_Origin.z) / localRay.m_Direction.z;
    if (t < EXR_EPSILON || t > ray.m_TMax)
        return false;

    float x = localRay(t).x;
    float y = localRay(t).y;

    if (x < -m_HalfExtents.x || x > m_HalfExtents.x || y < -m_HalfExtents.y || y > m_HalfExtents.y)
        return false;

    tHit = t;
    interaction->m_Point = m_Transform.GetMatrix() * localRay(t);
    interaction->m_Normal = m_Transform.GetMatrix() * exrVector3::Forward();
    interaction->m_ = m_Material.get();
    return true;
}

Interaction Quad::Sample(const exrPoint2& u) const
{
    Interaction it;

    exrFloat randomX = (Random::Uniform01() * 2 - 1) * m_HalfExtents.x;
    exrFloat randomY = (Random::Uniform01() * 2 - 1) * m_HalfExtents.y;

    exrPoint3 samplePoint = m_Transform.GetMatrix() * exrPoint3(randomX, randomY, 0);
    it.m_Point = samplePoint;
    it.m_Normal = m_Transform.GetMatrix() * exrVector3::Forward();
    it.m_Material = m_Material.get();

    return it;
}

exrFloat Quad::GetArea() const
{
    return m_HalfExtents.x * m_HalfExtents.y * 4;
}

AABB Quad::ComputeBoundingVolume()
{
    exrPoint3 globalMin = m_Transform.GetMatrix() * m_HalfExtents;
    exrPoint3 globalMax = m_Transform.GetMatrix() * -m_HalfExtents;

    // Swap ensure min is min and max is actually max, since we may have rotated the points above
    exrPoint3 realMin = exrPoint3(exrMin(globalMin.x, globalMax.x), exrMin(globalMin.y, globalMax.y), exrMin(globalMin.z, globalMax.z));
    exrPoint3 realMax = exrPoint3(exrMax(globalMin.x, globalMax.x), exrMax(globalMin.y, globalMax.y), exrMax(globalMin.z, globalMax.z));

    return AABB(realMin, realMax);
}

exrEND_NAMESPACE
