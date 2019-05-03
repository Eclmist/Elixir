#ifndef __MATH_BOUNDING_VOLUME_H__
#define __MATH_BOUNDING_VOLUME_H__

#include "point.h"
#include "ray.h"

class BoundingVolume
{
public:
    BoundingVolume(const Point3f& min = Point3f(-1.0f), const Point3f & max = Point3f(-1.0f))
        : m_Min(min), m_Max(max) {};

    Point3f Min() const { return m_Min; }
    Point3f Max() const { return m_Max; }

    bool Hit(const Ray& r, float tMin, float tMax);

private:
    Point3f m_Min;
    Point3f m_Max;
};

#endif // !__MATH_BOUNDING_VOLUME_H__
