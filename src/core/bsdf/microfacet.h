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

#include "bxdf.h"

exrBEGIN_NAMESPACE

class Microfacet : public BxDF
{
public:
    Microfacet(const exrSpectrum& r)
        : BxDF(BxDFType(BXDFTYPE_HAS_REFLECTANCE | BXDFTYPE_DIFFUSE))
        , m_Albedo(r) {};

    exrSpectrum f(const exrVector3& wo, const exrVector3& wi) const override;

    // Lambertian reflection is equal in all directions, so its hemispherical directional
    // reflectance is available in closed form.
    exrSpectrum rho(const exrVector3& wo, exrU32 numSamples) const override { return m_Albedo; }

private:
    exrSpectrum m_Albedo;
};
exrEND_NAMESPACE