#ifndef __MATERIAL_METALLIC_H__
#define __MATERIAL_METALLIC_H__

#include "material.h"

exrBEGIN_NAMESPACE

class Metallic : public Material
{
public:
    Metallic(const exrVector3& a, const exrVector3& r) : m_Albedo(a), m_Roughness(r) {}

    virtual exrBool Scatter(const Ray& incomingRay, const PrimitiveHitInfo& hitInfo, exrVector3& attenuation, Ray& scattered) const
    {
        exrVector3 reflected = Reflect(incomingRay.m_Direction.Normalized(), hitInfo.normal);
        scattered = Ray(hitInfo.point, reflected + m_Roughness * Random::RandomInUnitSphere());
        attenuation = m_Albedo;

        return (Dot(scattered.m_Direction, hitInfo.normal) > 0);
    }

public:
    exrVector3 m_Albedo;
    exrVector3 m_Roughness;
};

exrEND_NAMESPACE

#endif // !__MATERIAL_METALLIC_H__

