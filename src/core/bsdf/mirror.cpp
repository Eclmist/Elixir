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

#include "mirror.h"

exrBEGIN_NAMESPACE

exrSpectrum Mirror::f(const exrVector3& wo, const exrVector3& wi) const
{
    exrVector3 reflectDir = Reflect(-wo, exrVector3::Forward()).Normalized();
    // If angle of incidence != angle of reflection
    if (Dot(reflectDir, wi.Normalized()) < 1 - EXR_EPSILON)
        return 0;

    // Compute fresnel
    exrFloat vDotH = Dot(wi, exrVector3::Forward());
    return MicrofacetFresnel(m_Specular, vDotH);
}

exrSpectrum Mirror::Sample_f(const exrVector3& wo, exrVector3* wi, exrFloat* pdf) const
{
    // local space normal is always z forward
    *wi = Reflect(-wo, exrVector3::Forward());
    *pdf = 1;

    return f(wo, *wi);
}

exrSpectrum Mirror::rho(const exrVector3& wo, exrU32 numSamples) const
{
    return m_Specular * EXR_M_INVPI;
}

exrEND_NAMESPACE

