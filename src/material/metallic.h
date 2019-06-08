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

class Metallic : public Material
{
public:
    Metallic(const exrVector3& a, const exrVector3& r)
        : m_Albedo(a)
        , m_Roughness(r) {};

    Metallic(const Metallic& copy)
        : m_Albedo(copy.m_Albedo)
        , m_Roughness(copy.m_Roughness) {};

    virtual exrBool Scatter(const Ray& incomingRay, const Interaction& hitInfo, exrVector3& attenuation, Ray& scattered) const override
    {
        exrVector3 reflected = Reflect(incomingRay.m_Direction.Normalized(), hitInfo.m_Normal);
        scattered = Ray(hitInfo.m_Point, reflected + m_Roughness * Random::RandomInUnitSphere());
        attenuation = m_Albedo;

        return (Dot(scattered.m_Direction, hitInfo.m_Normal) > 0);
    };

public:
    exrVector3 m_Albedo;
    exrVector3 m_Roughness;
};

exrEND_NAMESPACE