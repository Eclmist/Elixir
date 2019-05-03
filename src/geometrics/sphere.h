#ifndef __GEOMETRICS_SPHERE_H__
#define __GEOMETRICS_SPHERE_H__

#include "geometry.h"

class Sphere: public Geometry
{
public:
    Sphere(Vector3f center, float radius, std::shared_ptr<Material> material) : m_Center(center), m_Radius(radius), m_Material(material) {};
    virtual bool Hit(const Ray& ray, float tMin, float tMax, GeometryHitInfo& hit) const;

public:
    const std::shared_ptr<Material> m_Material;

public:
    Vector3f m_Center;
    float m_Radius;
};
#endif // !__GEOMETRICS_SPHERE_H__

