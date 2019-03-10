#include "ray.h"

Ray::Ray(const Vector3f & ori, const Vector3f & dir, float dist)
{
    m_Origin = ori;
    m_Direction = dir;
    m_Distance = dist;
}

Ray::Ray(const Ray & copy)
{
    m_Origin = copy.m_Origin;
    m_Direction = copy.m_Direction;
    m_Distance = copy.m_Distance;
}
