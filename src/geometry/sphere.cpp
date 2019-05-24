#include "sphere.h"

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
            hit.T = hitPoint1;
            hit.Point = ray(hit.T);
            hit.Normal = (hit.Point - m_Center) / m_Radius;
            hit.Material = m_Material.get();
            return true;
        }

        if (hitPoint2 <= tMax && hitPoint2 >= tMin)
        {
            hit.T = hitPoint2;
            hit.Point = ray(hit.T);
            hit.Normal = (hit.Point - m_Center) / m_Radius;
            hit.Material = m_Material.get();
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