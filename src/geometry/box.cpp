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

bool Box::Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax, PrimitiveHitInfo& hit) const
{
    exrBool hasIntersect = false;
    Ray transformedRay = m_Transform.GetInverseMatrix() * ray;

    for (exrU32 i = 0; i < 6; i++)
    {
        if (m_Sides[i]->Intersect(transformedRay, tMin, tMax, hit))
        {
            tMax = hit.m_T;
            hasIntersect = true;
            hit.m_Normal = m_Normals[i];
            hit.m_Point = m_Transform.GetMatrix() * transformedRay(hit.m_T);
            hit.m_Material = m_Material.get();
        }
    }
    
    return hasIntersect;
}

bool Box::ComputeBoundingVolume()
{
    exrPoint globalMin = m_Transform.GetMatrix() * m_LocalMin;
    exrPoint globalMax = m_Transform.GetMatrix() * m_LocalMax;

    // Swap ensure min is min and max is actually max, since we may have rotated the points above
    exrPoint realMin = exrPoint(exrMin(globalMin.x, globalMax.x), exrMin(globalMin.y, globalMax.y), exrMin(globalMin.z, globalMax.z));
    exrPoint realMax = exrPoint(exrMax(globalMin.x, globalMax.x), exrMax(globalMin.y, globalMax.y), exrMax(globalMin.z, globalMax.z));

    m_BoundingVolume = BoundingVolume(realMin, realMax);
    return true;
}

exrEND_NAMESPACE