#ifndef __MATH_RAY_H__
#define __MATH_RAY_H__

#include "vector3.h"

class Ray
{
public:
    Ray() {}
    Ray(const Vector3f& ori, const Vector3f& dir, float dist) : m_Origin(ori), m_Direction(dir), m_Distance(dist) {}
    
    inline Vector3f GetOrigin() const { return m_Origin; }
    inline Vector3f GetDirection() const { return m_Direction; }
    inline Vector3f Evaluate(float t) const { return m_Origin + t * m_Direction; }

private:
    Vector3f m_Origin;
    Vector3f m_Direction;
    float m_Distance;

};
#endif // !__MATH_RAY_H__

