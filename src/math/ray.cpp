#include "ray.h"

Ray::Ray(const Ray& copy)
{
    m_Origin = copy.m_Origin;
    m_Direction = copy.m_Direction;
    m_Distance = copy.m_Distance;
}
