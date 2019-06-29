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

class Dielectric : public Material
{
public:
    Dielectric(const exrVector3& a, exrFloat ior) : m_Albedo(a), m_RefractiveIndex(ior) {};

    Dielectric(const Dielectric& copy)
        : m_Albedo(copy.m_Albedo)
        , m_RefractiveIndex(copy.m_RefractiveIndex) {};

    virtual exrBool Scatter(const Ray& incomingRay, const Interaction& hitInfo, exrVector3& attenuation, Ray& scattered) const override
    {
        throw std::exception("Use of outdated material!");
    };

public:
    exrVector3 m_Albedo;
    exrFloat m_RefractiveIndex;
};

exrEND_NAMESPACE