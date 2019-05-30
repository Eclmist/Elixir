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

class DiffuseLight : public Material
{
public:
    DiffuseLight(const exrVector3& e) : m_Emissive(e) {};

    DiffuseLight(const DiffuseLight& copy) : m_Emissive(copy.m_Emissive) {};

    virtual exrBool Scatter(const Ray& in, const PrimitiveHitInfo& hitInfo, exrVector3& attenuation, Ray& scattered) const override
    {
        // Assume light source does not scatter incoming ray
        return false;
    };

    virtual exrVector3 Emit(const Ray& in, const PrimitiveHitInfo& hitInfo) const override
    {
        if (Dot(in.m_Direction, hitInfo.m_Normal) < 0)
            return m_Emissive;
        else
            return 0.0f;
    };

private:
    exrVector3 m_Emissive;
};

exrEND_NAMESPACE