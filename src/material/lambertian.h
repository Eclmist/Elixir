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
        exrPoint target = hitInfo.Point + hitInfo.Normal + Random::RandomInUnitSphere();
        scattered = Ray(hitInfo.Point, target - hitInfo.Point);
        attenuation = m_Albedo;
        return true;
    };

public:
    exrVector3 m_Albedo;
};

exrEND_NAMESPACE