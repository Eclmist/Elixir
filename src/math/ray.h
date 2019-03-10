#ifndef __MATH_RAY_H__
#define __MATH_RAY_H__

#include <float.h>
#include "vector3.h"

class Ray
{
public:
    Ray() {}
    Ray(const Vector3f& ori, const Vector3f& dir, float dist = 1000.0f);
    Ray(const Ray& copy);
    
    inline Vector3f operator()(float t) const { return m_Origin + t * m_Direction; }
public:
    Vector3f m_Origin;
    Vector3f m_Direction;
    float m_Distance;
};
#endif // !__MATH_RAY_H__

