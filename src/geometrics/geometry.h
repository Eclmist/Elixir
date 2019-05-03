#ifndef __GEOMETRICS_GEOMETRY_H__
#define __GEOMETRICS_GEOMETRY_H__

#include <memory>
#include "math/ray.h"

class Material;

struct GeometryHitInfo
{
    float t;
    Vector3f point;
    Vector3f normal;
    std::shared_ptr<Material> material;
};

class Geometry
{
public:
    virtual bool Hit(const Ray& ray, float tMin, float tMax, GeometryHitInfo& hit) const = 0;
};

#endif // !__GEOMETRICS_GEOMETRY_H__

