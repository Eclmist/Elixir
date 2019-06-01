/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019 Samuel Van Allen - All rights reserved.

    Elixir is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

    virtual exrBool Scatter(const Ray& incomingRay, const Interaction& hitInfo, exrVector3& attenuation, Ray& scattered, exrFloat& pdf) const override
    {
        exrVector3 reflectedRay = Reflect(incomingRay.m_Direction, hitInfo.m_Normal);

        exrFloat ni_over_nt;
        attenuation = m_Albedo;

        exrVector3 outNormal;
        exrVector3 refractedRay;

        // for Fresnel calculations
        exrFloat cosine;
        exrFloat reflectionProbability;

        // entering material
        if (Dot(incomingRay.m_Direction, hitInfo.m_Normal) > 0.0f)
        {
            outNormal = -hitInfo.m_Normal;
            ni_over_nt = m_RefractiveIndex;
            cosine = m_RefractiveIndex * Dot(incomingRay.m_Direction, hitInfo.m_Normal) / incomingRay.m_Direction.Magnitude();
        }
        else // exiting material
        {
            outNormal = hitInfo.m_Normal;
            ni_over_nt = 1.0f / m_RefractiveIndex;
            cosine = -Dot(incomingRay.m_Direction, hitInfo.m_Normal) / incomingRay.m_Direction.Magnitude();
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
            scattered = Ray(hitInfo.m_Point, reflectedRay);
        }
        else
        {
            scattered = Ray(hitInfo.m_Point, refractedRay);
        }

        return true;
    };

public:
    exrVector3 m_Albedo;
    exrFloat m_RefractiveIndex;
};

exrEND_NAMESPACE