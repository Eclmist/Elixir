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

bool Sphere::Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax, PrimitiveHitInfo& hit) const
{
    Ray transformedRay = m_Transform.GetInverseMatrix() * ray;
    exrVector3 r0 = transformedRay.m_Origin - exrPoint::Zero();
    exrFloat a = Dot(transformedRay.m_Direction, transformedRay.m_Direction);
    exrFloat b = Dot(r0, transformedRay.m_Direction);
    exrFloat c = Dot(r0, r0) - (0.25f);
    exrFloat discriminant = b * b - a * c;

    // solve quadratic equation to find t
    if (discriminant > 0)
    {
        exrFloat hitPoint1 = (-b - sqrt(b * b - a * c)) / a;
        exrFloat hitPoint2 = (-b + sqrt(b * b - a * c)) / a;

        if (hitPoint1 <= tMax && hitPoint1 >= tMin)
        {
            hit.m_T = hitPoint1;
            hit.m_Point = m_Transform.GetMatrix() * transformedRay(hitPoint1);
            hit.m_Normal = m_Transform.GetInverseMatrix().Transposed() * ((transformedRay(hitPoint1) - exrPoint::Zero()) / 0.5f);
            hit.m_Material = m_Material.get();
            return true;
        }

        if (hitPoint2 <= tMax && hitPoint2 >= tMin)
        {
            hit.m_T = hitPoint2;
            hit.m_Point = m_Transform.GetMatrix() * transformedRay(hitPoint2);
            hit.m_Normal = m_Transform.GetInverseMatrix().Transposed() * ((transformedRay(hitPoint2) - exrPoint::Zero()) / 0.5f);
            hit.m_Material = m_Material.get();
            return true;
        }
    }

    // no real value, did not intersect sphere
    return false;
}

bool Sphere::ComputeBoundingVolume()
{
    exrPoint min = m_Transform.GetMatrix() * exrPoint(-0.5f);
    exrPoint max = m_Transform.GetMatrix() * exrPoint(0.5f);

    m_BoundingVolume = BoundingVolume(min, max);
    return true;
}

exrEND_NAMESPACE