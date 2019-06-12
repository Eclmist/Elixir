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

#include "sphere.h"

exrBEGIN_NAMESPACE

exrBool Sphere::Intersect(const Ray& ray, exrFloat& tHit, SurfaceInteraction* hit) const
{
    Ray localRay = m_Transform.GetInverseMatrix() * ray;
    exrVector3 r0 = localRay.m_Origin - exrPoint3::Zero();
    exrFloat a = Dot(localRay.m_Direction, localRay.m_Direction);
    exrFloat b = 2 * Dot(r0, localRay.m_Direction);
    exrFloat c = Dot(r0, r0) - (m_Radius * m_Radius);

    // solve quadratic equation to find t
    exrFloat t0, t1;
    if (!exrQuadratic(a, b, c, &t0, &t1))
        return false;

    if (t0 > ray.m_TMax || t1 <= EXR_EPSILON)
        return false;

    if (t0 <= EXR_EPSILON)
        t0 = t1;

    tHit = t0;
    hit->m_Point = m_Transform.GetMatrix() * localRay(t0);
    // Normal vectors on sphere do not have to be transformed as sphere have no rotation and only uniform scale
    hit->m_Normal = static_cast<exrVector3>(localRay(t0)) / m_Radius;
    hit->m_Wo = -ray.m_Direction;
    hit->m_BSDF = m_Material.CreateB;

    return true;
}

exrBool Sphere::HasIntersect(const Ray& ray, exrFloat& tHit) const
{
    Ray localRay = m_Transform.GetInverseMatrix() * ray;
    exrVector3 r0 = localRay.m_Origin - exrPoint3::Zero();
    exrFloat a = Dot(localRay.m_Direction, localRay.m_Direction);
    exrFloat b = 2 * Dot(r0, localRay.m_Direction);
    exrFloat c = Dot(r0, r0) - (m_Radius * m_Radius);

    // solve quadratic equation to find t
    exrFloat t0, t1;
    if (!exrQuadratic(a, b, c, &t0, &t1))
        return false;

    if (t0 > ray.m_TMax || t1 <= EXR_EPSILON)
        return false;

    if (t0 <= EXR_EPSILON)
        t0 = t1;

    tHit = t0;

    return true;
}

AABB Sphere::ComputeBoundingVolume()
{
    exrPoint3 min = m_Transform.GetMatrix() * exrPoint3(-m_Radius);
    exrPoint3 max = m_Transform.GetMatrix() * exrPoint3(m_Radius);
    return AABB(min, max);
}

Interaction Sphere::Sample(const exrPoint2& u) const
{
    exrPoint3 pointOnSphere = exrPoint3::Zero() + m_Radius * Random::RandomOnUnitSphere();
    Interaction it;
    // Normal vectors on sphere do not have to be transformed as sphere have no rotation and only uniform scale
    it.m_Normal = static_cast<exrVector3>(pointOnSphere).Normalized();
    it.m_Point = m_Transform.GetMatrix() * pointOnSphere;
    return it;
}

exrFloat Sphere::GetArea() const
{
    return 4 * EXR_M_PI * m_Radius * m_Radius;
}

exrEND_NAMESPACE

