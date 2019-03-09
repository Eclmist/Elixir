#include "ray.h"

Ray::Ray(const Vector3f & ori, const Vector3f & dir, float dist, float tmin, float tmax)
{
    m_Origin = ori;
    m_Direction = dir;
    m_Distance = dist;
    m_tMin = tmin;
    m_tMax = tmax;
}

Ray::Ray(const Ray & copy)
{
    m_Origin = copy.m_Origin;
    m_Direction = copy.m_Direction;
    m_Distance = copy.m_Distance;
    m_tMin = copy.m_tMin;
    m_tMax = copy.m_tMax;
}
