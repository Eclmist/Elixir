#pragma once

#include "material.h"

exrBEGIN_NAMESPACE

class Metallic : public Material
{
public:
    Metallic(const exrVector3& a, const exrVector3& r) : m_Albedo(a), m_Roughness(r) {};

    Metallic(const Metallic& copy) : m_Albedo(copy.m_Albedo), m_Roughness(copy.m_Roughness) {};

    virtual exrBool Scatter(const Ray& incomingRay, const PrimitiveHitInfo& hitInfo, exrVector3& attenuation, Ray& scattered) const
    {
        exrVector3 reflected = Reflect(incomingRay.m_Direction.Normalized(), hitInfo.Normal);
        scattered = Ray(hitInfo.Point, reflected + m_Roughness * Random::RandomInUnitSphere());
        attenuation = m_Albedo;

        return (Dot(scattered.m_Direction, hitInfo.Normal) > 0);
    };

public:
    exrVector3 m_Albedo;
    exrVector3 m_Roughness;
};

exrEND_NAMESPACE