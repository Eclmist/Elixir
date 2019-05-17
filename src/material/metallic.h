#ifndef __MATERIAL_METALLIC_H__
#define __MATERIAL_METALLIC_H__

#include "material.h"

class Metallic : public Material
{
public:
    Metallic(const Vector3f& a, const Vector3f& r) : m_Albedo(a), m_Roughness(r) {}

    virtual bool Scatter(const Ray& incomingRay, const PrimitiveHitInfo& hitInfo, Vector3f& attenuation, Ray& scattered) const
    {
        Vector3f reflected = Reflect(incomingRay.m_Direction.Normalized(), hitInfo.normal);
        scattered = Ray(hitInfo.point, reflected + m_Roughness * RandomInUnitSphere());
        attenuation = m_Albedo;

        return (Dot(scattered.m_Direction, hitInfo.normal) > 0);
    }

public:
    Vector3f m_Albedo;
    Vector3f m_Roughness;
};

#endif // !__MATERIAL_METALLIC_H__

