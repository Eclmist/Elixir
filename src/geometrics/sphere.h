#ifndef __GEOMETRICS_SPHERE_H__
#define __GEOMETRICS_SPHERE_H__

#include "geometry.h"

class Sphere: public Geometry
{
public:
    Sphere() {}
    Sphere(Vector3f center, float radius) : m_Center(center), m_Radius(radius) {};

    virtual bool Hit(const Ray& ray, RayHitRecord& hit) const;

public:
    Vector3f m_Center;
    float m_Radius;
};
#endif // !__GEOMETRICS_SPHERE_H__

