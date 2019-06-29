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

#include "aabb.h"
#include "core/primitive/primitive.h"

exrBEGIN_NAMESPACE

exrBool AABB::Intersect(const Ray& r) const
{
    exrFloat tMin = EXR_EPSILON;
    exrFloat tMax = r.m_TMax;

    for (int i = 0; i < 3; ++i)
    {
        exrFloat invD = 1.0f / r.m_Direction[i];
        exrFloat t0 = (m_Min[i] - r.m_Origin[i]) * invD;
        exrFloat t1 = (m_Max[i] - r.m_Origin[i]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        tMin = t0 > tMin ? t0 : tMin;
        tMax = t1 < tMax ? t1 : tMax;

        if (tMax <= tMin)
            return false;
    }

    return true;
}

AABB AABB::Union(const AABB& bv1, const AABB& bv2)
{
    exrFloat minX, minY, minZ;
    exrFloat maxX, maxY, maxZ;

    minX = exrMin(bv1.Min().x, bv2.Min().x);
    minY = exrMin(bv1.Min().y, bv2.Min().y);
    minZ = exrMin(bv1.Min().z, bv2.Min().z);

    maxX = exrMax(bv1.Max().x, bv2.Max().x);
    maxY = exrMax(bv1.Max().y, bv2.Max().y);
    maxZ = exrMax(bv1.Max().z, bv2.Max().z);

    return AABB(exrPoint3(minX, minY, minZ), exrPoint3(maxX, maxY, maxZ));
}

AABB AABB::BoundPrimitives(const std::vector<Primitive*>& primitives)
{
    // We CANNOT combine with exrPoint3(0) because that will make all bv extend to origin..
    if (primitives.size() <= 0)
    {
        return AABB(exrPoint3::Zero(), exrPoint3::Zero());
    }

    AABB combinedBv = primitives[0]->GetBoundingVolume();

    for (int i = 1; i < primitives.size(); ++i)
    {
        combinedBv = Union(combinedBv, primitives[i]->GetBoundingVolume());
    }

    return combinedBv;
}

exrEND_NAMESPACE