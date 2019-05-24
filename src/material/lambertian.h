#pragma once

#include "material.h"

exrBEGIN_NAMESPACE

class Lambertian : public Material
{
public:
    Lambertian(const exrVector3& a) : m_Albedo(a) {};

    Lambertian(const Lambertian& copy) : m_Albedo(copy.m_Albedo) {};

    virtual exrBool Scatter(const Ray& incomingRay, const PrimitiveHitInfo& hitInfo, exrVector3& attenuation, Ray& scattered) const
    {
        exrPoint target = hitInfo.point + hitInfo.normal + Random::RandomInUnitSphere();
        scattered = Ray(hitInfo.point, target - hitInfo.point);
        attenuation = m_Albedo;
        return true;
    };

public:
    exrVector3 m_Albedo;
};

exrEND_NAMESPACE