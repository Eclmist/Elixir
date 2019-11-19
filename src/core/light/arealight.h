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

#include "light.h"

exrBEGIN_NAMESPACE

class AreaLight : public Light
{
public:
    AreaLight(const Transform& transform)
        : Light(transform, Light::LightFlags::LIGHTFLAGS_AREA) {}

    AreaLight(const AreaLight& copy)
        : Light(copy.m_Transform, copy.m_Flags) {}

    virtual exrSpectrum Sample_f(const Interaction& interaction, const exrVector3& w) const = 0;

    exrSpectrum Sample_f(const Interaction& interaction, const exrPoint2& uv, exrVector3& wi, exrFloat& pdf) const override
    {
        throw std::logic_error("The method or operation is not implemented.");
    }

    exrSpectrum Power() const override
    {
        throw std::logic_error("The method or operation is not implemented.");
    }

protected:
    exrU32 m_NumSamples;
};

exrEND_NAMESPACE