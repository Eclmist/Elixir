#ifndef __MATERIAL_LAMBERTIAN_H__
#define __MATERIAL_LAMBERTIAN_H__

#include "material.h"

class Lambertian : public Material
{
public:
    Lambertian(const Vector3f& a) : m_Albedo(a) {}

    virtual bool Scatter(const Ray& incomingRay, const GeometryHitInfo& hitInfo, Vector3f& attenuation, Ray& scattered) const 
    {
        Vector3f target = hitInfo.point + hitInfo.normal + RandomInUnitSphere();
        scattered = Ray(hitInfo.point, target - hitInfo.point);
        attenuation = m_Albedo;
        return true;
    }

public:
    Vector3f m_Albedo;

};

#endif // !__MATERIAL_LAMBERTIAN_H__

