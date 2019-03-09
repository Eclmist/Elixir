#ifndef __CORE_GEOMETRY_H__
#define __CORE_GEOMETRY_H__

#include "math/ray.h"

struct RayHitRecord
{
    float t;
    Vector3f point;
    Vector3f normal;
};

class Geometry
{
public:
    virtual bool Hit(const Ray& ray, RayHitRecord& hit) const = 0;
};

#endif // !__CORE_GEOMETRY_H__

