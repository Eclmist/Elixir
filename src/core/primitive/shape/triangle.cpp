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

#include "triangle.h"

exrBEGIN_NAMESPACE

exrBool Triangle::Intersect(const Ray& ray, exrFloat& tHit, SurfaceInteraction* interaction) const
{

}

exrBool Triangle::HasIntersect(const Ray& ray, exrFloat& tHit) const
{
    const Vertex* v1 = m_SharedMesh->GetVertexAtIndex(m_Indices[0]);
    const Vertex* v2 = m_SharedMesh->GetVertexAtIndex(m_Indices[1]);
    const Vertex* v3 = m_SharedMesh->GetVertexAtIndex(m_Indices[2]);

    exrPoint3 p0t = v1->m_Position - exrVector3(ray.m_Origin);
    exrPoint3 p1t = v2->m_Position - exrVector3(ray.m_Origin);
    exrPoint3 p2t = v3->m_Position - exrVector3(ray.m_Origin);
}

AABB Triangle::ComputeBoundingVolume() const
{
    const Vertex* v1 = m_SharedMesh->GetVertexAtIndex(m_Indices[0]);
    const Vertex* v2 = m_SharedMesh->GetVertexAtIndex(m_Indices[1]);
    const Vertex* v3 = m_SharedMesh->GetVertexAtIndex(m_Indices[2]);

    exrFloat xMin = exrMin(exrMin(v1->m_Position.x, v2->m_Position.x), v3->m_Position.x);
    exrFloat yMin = exrMin(exrMin(v1->m_Position.y, v2->m_Position.y), v3->m_Position.y);
    exrFloat zMin = exrMin(exrMin(v1->m_Position.z, v2->m_Position.z), v3->m_Position.z);

    exrFloat xMax = exrMax(exrMax(v1->m_Position.x, v2->m_Position.x), v3->m_Position.x);
    exrFloat yMax = exrMax(exrMax(v1->m_Position.y, v2->m_Position.y), v3->m_Position.y);
    exrFloat zMax = exrMax(exrMax(v1->m_Position.z, v2->m_Position.z), v3->m_Position.z);

    return AABB(exrPoint3(xMin, yMin, zMin), exrPoint3(xMax, yMax, zMax));
}

exrEND_NAMESPACE

