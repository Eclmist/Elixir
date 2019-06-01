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

exrEND_NAMESPACE