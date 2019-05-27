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
    Ray transformedRay = ray;

    // Transform ray into local space
    transformedRay.m_Origin = m_Transform.GetInverseMatrix() * ray.m_Origin;
    transformedRay.m_Direction = (m_Transform.GetInverseMatrix() * ray.m_Direction).Normalized();

    exrU32 normalAxis = 0;

    for (exrU32 i = 0; i < 3; i++)
    {
        exrFloat invD = 1.0f / ray.m_Direction[i];
        exrFloat t0 = (m_LocalMin[i] - transformedRay.m_Origin[i]) * invD;
        exrFloat t1 = (m_LocalMax[i] - transformedRay.m_Origin[i]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);

        tMax = t1 < tMax ? t1 : tMax;
        
        // Update hit point to more accurate axis
        if (t0 > tMin)
        {
            tMin = t0;
            hit.m_T = t0;
            hit.m_Point = transformedRay(t0);
            hit.m_Normal.x = (i == 0) ? (invD < 0.0f ? -1.0f : 1.0f) : 0.0f;
            hit.m_Normal.y = (i == 1) ? (invD < 0.0f ? -1.0f : 1.0f) : 0.0f;
            hit.m_Normal.z = (i == 2) ? (invD < 0.0f ? -1.0f : 1.0f) : 0.0f;
        }

        if (tMax <= tMin)
            return false;
    }

    hit.m_Point = m_Transform.GetMatrix() * hit.m_Point;
   // hit.m_Normal = m_Transform.GetMatrix() * hit.m_Normal;
    hit.m_Material = m_Material.get();
    
    return true;
}

bool Box::ComputeBoundingVolume()
{
    exrPoint min = m_Transform.GetMatrix() * exrPoint(-0.5f);
    exrPoint max = m_Transform.GetMatrix() * exrPoint(0.5f);

    m_BoundingVolume = BoundingVolume(min, max);
    return true;
}

exrEND_NAMESPACE