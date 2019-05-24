#pragma once

#include "material.h"
#include "math/utils.h"

exrBEGIN_NAMESPACE

class Dielectric : public Material
{
public:
    Dielectric(const exrVector3& a, exrFloat ior) : m_Albedo(a), m_RefractiveIndex(ior) {};

    Dielectric(const Dielectric& copy)
        : m_Albedo(copy.m_Albedo)
        , m_RefractiveIndex(copy.m_RefractiveIndex) {};

    virtual exrBool Scatter(const Ray& incomingRay, const PrimitiveHitInfo& hitInfo, exrVector3& attenuation, Ray& scattered) const
    {
        exrVector3 reflectedRay = Reflect(incomingRay.m_Direction, hitInfo.Normal);

        exrFloat ni_over_nt;
        attenuation = m_Albedo;

        exrVector3 outNormal;
        exrVector3 refractedRay;

        // for Fresnel calculations
        exrFloat cosine;
        exrFloat reflectionProbability;

        // entering material
        if (Dot(incomingRay.m_Direction, hitInfo.Normal) > 0.0f)
        {
            outNormal = -hitInfo.Normal;
            ni_over_nt = m_RefractiveIndex;
            cosine = m_RefractiveIndex * Dot(incomingRay.m_Direction, hitInfo.Normal) / incomingRay.m_Direction.Magnitude();
        }
        else // exiting material
        {
            outNormal = hitInfo.Normal;
            ni_over_nt = 1.0f / m_RefractiveIndex;
            cosine = -Dot(incomingRay.m_Direction, hitInfo.Normal) / incomingRay.m_Direction.Magnitude();
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
            scattered = Ray(hitInfo.Point, reflectedRay);
        }
        else
        {
            scattered = Ray(hitInfo.Point, refractedRay);
        }

        return true;
    };

public:
    exrVector3 m_Albedo;
    exrFloat m_RefractiveIndex;
};

exrEND_NAMESPACE