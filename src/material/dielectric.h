#ifndef __MATERIAL_DIELECTRIC_H__
#define __MATERIAL_DIELECTRIC_H__

#include "material/material.h"

exrBEGIN_NAMESPACE

class Dielectric : public Material
{
public:
    Dielectric(const exrVector3& a, exrFloat ior) : m_Albedo(a), m_RefractiveIndex(ior) {}

    virtual exrBool Scatter(const Ray& incomingRay, const PrimitiveHitInfo& hitInfo, exrVector3& attenuation, Ray& scattered) const
    {
        exrVector3 reflectedRay = Reflect(incomingRay.m_Direction, hitInfo.normal);

        exrFloat ni_over_nt;
        attenuation = m_Albedo;

        exrVector3 outNormal;
        exrVector3 refractedRay;

        // for Fresnel calculations
        exrFloat cosine;
        exrFloat reflectionProbability;

        // entering material
        if (Dot(incomingRay.m_Direction, hitInfo.normal) > 0.0f)
        {
            outNormal = -hitInfo.normal;
            ni_over_nt = m_RefractiveIndex;
            cosine = m_RefractiveIndex * Dot(incomingRay.m_Direction, hitInfo.normal) / incomingRay.m_Direction.Magnitude();
        }
        else // exiting material
        {
            outNormal = hitInfo.normal;
            ni_over_nt = 1.0f / m_RefractiveIndex;
            cosine = -Dot(incomingRay.m_Direction, hitInfo.normal) / incomingRay.m_Direction.Magnitude();
        }

        if (Refract(incomingRay.m_Direction, outNormal, ni_over_nt, refractedRay))
        {
            // refraction
            reflectionProbability = SchlickFresnelApproximation(cosine, m_RefractiveIndex);
        }
        else
        {
            // total internal reflection
            reflectionProbability = 1.0f;
        }

        // Reflection based on Fresnel approximation
        if (Random::Random01() < reflectionProbability)
        {
            scattered = Ray(hitInfo.point, reflectedRay);
        }
        else
        {
            scattered = Ray(hitInfo.point, refractedRay);
        }

        return true;
    }

public:
    exrVector3 m_Albedo;
    exrFloat m_RefractiveIndex;
};

exrEND_NAMESPACE

#endif // !__MATERIAL_DIELECTRIC_H__

