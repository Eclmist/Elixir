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

class OrenNayar : public BxDF
{
public:
    OrenNayar(const exrSpectrum& r, exrFloat sigma)
        : BxDF(BxDFType(BXDFTYPE_HAS_REFLECTANCE | BXDFTYPE_DIFFUSE))
        , m_Albedo(r)
        , m_Sigma(exrDegToRad(sigma))
    {
        exrFloat sigma2 = exrDegToRad(sigma);
        m_A = 1.0f - (sigma2 / (2.0f * (sigma2 + 0.33f)));
        m_B = 0.45f * sigma2 / (sigma2 + 0.09f);
    };

    exrSpectrum f(const exrVector3& wo, const exrVector3& wi) const override;
    exrSpectrum rho(const exrVector3& wo, exrU32 numSamples) const override { return m_Albedo; }

private:
    exrFloat m_Sigma;
    exrFloat m_A;
    exrFloat m_B;
    exrSpectrum m_Albedo;
};
exrEND_NAMESPACE