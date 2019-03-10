#ifndef __GEOMETRICS_SPHERE_H__
#define __GEOMETRICS_SPHERE_H__

#include "geometry.h"

class Sphere: public Geometry
{
public:
    Sphere() {}
    Sphere(Vector3f center, float radius, Material* material) : m_Center(center), m_Radius(radius), m_pMaterial(material) {};

    virtual bool Hit(const Ray& ray, float tMin, float tMax, GeometryHitInfo& hit) const;

public:
    Material* m_pMaterial;

public:
    Vector3f m_Center;
    float m_Radius;
};
#endif // !__GEOMETRICS_SPHERE_H__

