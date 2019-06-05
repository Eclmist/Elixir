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

exrBEGIN_NAMESPACE

class Lambertian : public Material
{
public:
    Lambertian(const exrVector3& a)
        : m_Albedo(a) {};

    Lambertian(const Lambertian& copy)
        : m_Albedo(copy.m_Albedo) {};

    virtual exrBool Scatter(const Ray& incomingRay, const Interaction& hitInfo, exrVector3& attenuation, Ray& scattered) const override
    {
        exrVector3 cosDir = Random::CosineSampleHemisphere(Random::Uniform01Point2());
        exrVector3 refNormal = exrVector3::Up();
        exrPoint3 target = hitInfo.m_Point + hitInfo.m_Normal + (cosDir - refNormal);
        scattered = Ray(hitInfo.m_Point, target - hitInfo.m_Point);
        attenuation = m_Albedo;
        return true;
    };

public:
    exrVector3 m_Albedo;
};

exrEND_NAMESPACE