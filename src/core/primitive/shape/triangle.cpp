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

// Möller–Trumbore ray triangle intersection:
// https://cadxfem.org/inf/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
exrBool Triangle::Intersect(const Ray& ray, exrFloat& tHit, SurfaceInteraction* interaction) const
{
    exrVector3 e1, e2, p, q, t;
    Vertex v0, v1, v2;
    m_SharedMesh->GetVertexAtIndex(m_IndexInMesh, v0, v1, v2);

    e1 = v1.m_Position - v0.m_Position;
    e2 = v2.m_Position - v0.m_Position;

    p = Cross(ray.m_Direction, e2);
    exrFloat det = Dot(e1, p);

    if (abs(det) < EXR_EPSILON)
        return false;

    exrFloat invDet = 1 / det;
    
    // Calculate distance from v0 to ray origin
    t = ray.m_Origin - v0.m_Position;

    // Barycentric coordinates
    exrFloat u, v;

    u = Dot(t, p) * invDet;
    // Barycentric coordinates lie outside bounds (no intersect)
    if (u < 0 || u > 1)
        return false;

    q = Cross(t, e1);

    v = Dot(ray.m_Direction, q) * invDet;
    // Barycentric coordinates lie outside bounds (no intersect)
    if (v < 0 || u + v > 1)
        return false;

    // Compute normal vector
    exrVector3 normal = 
        (1 - u - v) * v0.m_Normal +
        u * v1.m_Normal +
        v * v2.m_Normal;

    exrFloat temp = Dot(e2, q) * invDet;

    if (temp < 0 || temp > ray.m_TMax)
        return false;

    tHit = temp;
    interaction->m_Point = ray(tHit);
    interaction->m_Normal = normal.Normalized();
    interaction->m_Wo = -ray.m_Direction;
    interaction->m_Shape = this;

    return true;
}

exrBool Triangle::HasIntersect(const Ray& ray, exrFloat& tHit) const
{
    exrVector3 e1, e2, p, q, t;

    Vertex v0, v1, v2;
    m_SharedMesh->GetVertexAtIndex(m_IndexInMesh, v0, v1, v2);

    e1 = v1.m_Position - v0.m_Position;
    e2 = v2.m_Position - v0.m_Position;

    p = Cross(ray.m_Direction, e2);
    exrFloat det = Dot(e1, p);

    if (abs(det) < EXR_EPSILON)
        return false;

    exrFloat invDet = 1 / det;
    
    // Calculate distance from v0 to ray origin
    t = ray.m_Origin - v0.m_Position;

    // Barycentric coordinates
    exrFloat u, v;

    u = Dot(t, p) * invDet;
    // Barycentric coordinates lie outside bounds (no intersect)
    if (u < 0 || u > 1)
        return false;

    q = Cross(t, e1);

    v = Dot(ray.m_Direction, q) * invDet;
    // Barycentric coordinates lie outside bounds (no intersect)
    if (v < 0 || u + v > 1)
        return false;

    exrFloat temp = Dot(e2, q) * invDet;

    if (temp < 0 || temp > ray.m_TMax)
        return false;

    tHit = temp;
    return true;
}

AABB Triangle::ComputeBoundingVolume() const
{
    Vertex v0, v1, v2;
    m_SharedMesh->GetVertexAtIndex(m_IndexInMesh, v0, v1, v2);

    exrFloat xMin = exrMin(exrMin(v0.m_Position.x, v1.m_Position.x), v2.m_Position.x);
    exrFloat yMin = exrMin(exrMin(v0.m_Position.y, v1.m_Position.y), v2.m_Position.y);
    exrFloat zMin = exrMin(exrMin(v0.m_Position.z, v1.m_Position.z), v2.m_Position.z);

    exrFloat xMax = exrMax(exrMax(v0.m_Position.x, v1.m_Position.x), v2.m_Position.x);
    exrFloat yMax = exrMax(exrMax(v0.m_Position.y, v1.m_Position.y), v2.m_Position.y);
    exrFloat zMax = exrMax(exrMax(v0.m_Position.z, v1.m_Position.z), v2.m_Position.z);

    return AABB(exrPoint3(xMin, yMin, zMin), exrPoint3(xMax, yMax, zMax));
}

exrEND_NAMESPACE

