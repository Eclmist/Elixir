#ifndef __GEOMETRICS_GEOMETRY_H__
#define __GEOMETRICS_GEOMETRY_H__

#include "math/ray.h"

class Material;

struct GeometryHitInfo
{
    float t;
    Vector3f point;
    Vector3f normal;
    Material* pMaterial;
};

class Geometry
{
public:
    virtual bool Hit(const Ray& ray, GeometryHitInfo& hit) const = 0;
};

#endif // !__GEOMETRICS_GEOMETRY_H__

