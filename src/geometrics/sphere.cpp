#include "sphere.h"

bool Sphere::Hit(const Ray & ray, RayHitRecord & hit) const
{
    Vector3f oc = ray.m_Origin - m_Center;
    float a = Dot(ray.m_Direction, ray.m_Direction);
    float b = Dot(oc, ray.m_Direction);
    float c = Dot(oc, oc) - m_Radius * m_Radius;
    float discriminant = b * b - a * c;

    // solve quadratic equation to find t
    if (discriminant > 0)
    {
        float hitPoint1 = (-b - sqrt(b * b - a * c)) / a;
        float hitPoint2 = (-b + sqrt(b * b - a * c)) / a;

        if (hitPoint1 <= ray.m_tMax && hitPoint1 >= ray.m_tMin)
        {
            hit.t = hitPoint1;
            hit.point = ray(hit.t);
            hit.normal = (hit.point - m_Center) / m_Radius;
            return true;
        }

        if (hitPoint2 <= ray.m_tMax && hitPoint2 >= ray.m_tMin)
        {
            hit.t = hitPoint2;
            hit.point = ray(hit.t);
            hit.normal = (hit.point - m_Center) / m_Radius;
            return true;
        }
    }

    // no real value, did not intersect sphere
    return false;
}
