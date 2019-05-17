#ifndef __MATERIAL_DIELECTRIC_H__
#define __MATERIAL_DIELECTRIC_H__

#include "material/material.h"

class Dielectric : public Material
{
public:
    Dielectric(const Vector3f& a, float ior) : m_Albedo(a), m_RefractiveIndex(ior) {}

    virtual bool Scatter(const Ray& incomingRay, const PrimitiveHitInfo& hitInfo, Vector3f& attenuation, Ray& scattered) const
    {
        Vector3 reflectedRay = Reflect(incomingRay.m_Direction, hitInfo.normal);

        float ni_over_nt;
        attenuation = m_Albedo;

        Vector3f outNormal;
        Vector3f refractedRay;

        // for Fresnel calculations
        float cosine;
        float reflectionProbability;

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
        if (RAND01() < reflectionProbability)
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
    Vector3f m_Albedo;
    float m_RefractiveIndex;
};

#endif // !__MATERIAL_DIELECTRIC_H__

