#include "sphere.h"

#include "material/material.h"
#include "math/ray.h"

exrBEGIN_NAMESPACE

bool Sphere::Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax, PrimitiveHitInfo& hit) const
{
    exrVector3 oc = ray.m_Origin - m_Center;
    exrFloat a = Dot(ray.m_Direction, ray.m_Direction);
    exrFloat b = Dot(oc, ray.m_Direction);
    exrFloat c = Dot(oc, oc) - m_Radius * m_Radius;
    exrFloat discriminant = b * b - a * c;

    // solve quadratic equation to find t
    if (discriminant > 0)
    {
        exrFloat hitPoint1 = (-b - sqrt(b * b - a * c)) / a;
        exrFloat hitPoint2 = (-b + sqrt(b * b - a * c)) / a;

        if (hitPoint1 <= tMax && hitPoint1 >= tMin)
        {
            hit.t = hitPoint1;
            hit.point = ray(hit.t);
            hit.normal = (hit.point - m_Center) / m_Radius;
            hit.material = m_Material.get();
            return true;
        }

        if (hitPoint2 <= tMax && hitPoint2 >= tMin)
        {
            hit.t = hitPoint2;
            hit.point = ray(hit.t);
            hit.normal = (hit.point - m_Center) / m_Radius;
            hit.material = m_Material.get();
            return true;
        }
    }

    // no real value, did not intersect sphere
    return false;
}

bool Sphere::ComputeBoundingVolume()
{
    exrPoint min = exrPoint(m_Center.x - m_Radius, m_Center.y - m_Radius, m_Center.z - m_Radius);
    exrPoint max = exrPoint(m_Center.x + m_Radius, m_Center.y + m_Radius, m_Center.z + m_Radius);
    m_BoundingVolume = BoundingVolume(min, max);
    return true;
}

exrEND_NAMESPACE