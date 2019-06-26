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

#include "core/elixir.h"

exrBEGIN_NAMESPACE

static const int numCIESamples = 471;
extern const exrFloat CIE_X[numCIESamples];
extern const exrFloat CIE_Y[numCIESamples];
extern const exrFloat CIE_Z[numCIESamples];
extern const exrFloat CIE_WavelengthsRaw[numCIESamples];

template <exrU32 numSpectrumSamples>
class Spectrum
{
public:
    Spectrum(exrFloat v = 0.0f);

    Spectrum& operator+=(const Spectrum& s2);
    Spectrum& operator-=(const Spectrum& s2);
    Spectrum& operator*=(const Spectrum& s2);
    Spectrum& operator/=(const Spectrum& s2);
    Spectrum operator+(const Spectrum& s2) const;
    Spectrum operator-(const Spectrum& s2) const;
    Spectrum operator*(const Spectrum& s2) const;
    Spectrum operator/(const Spectrum& s2) const;

    // This implies that the set of coefficients are linear
    inline exrFloat& operator[](exrU32 i) { return m_Wavelengths[i]; }

public:
    virtual exrVector3 ToXYZ();
    virtual exrVector3 ToRGB();

    // Conversion to RGB based on a standard set of RGB spectra that has been
    // defined for high-definition televisions
    inline void XYZToRGB(const exrFloat xyz[3], exrFloat& rgb[3])
    {
        rgb[0] = 3.240479f*xyz[0] - 1.537150f*xyz[1] - 0.498535f*xyz[2];
        rgb[1] = -0.969256f*xyz[0] + 1.875991f*xyz[1] + 0.041556f*xyz[2];
        rgb[2] = 0.055648f*xyz[0] - 0.204043f*xyz[1] + 1.057311f*xyz[2];
    };

    // The inverse of the xyz to rgb conversion
    inline void RGBToXYZ(const exrFloat rgb[3], exrFloat& xyz[3])
    {
        xyz[0] = 0.412453f*rgb[0] + 0.357580f*rgb[1] + 0.180423f*rgb[2];
        xyz[1] = 0.212671f*rgb[0] + 0.715160f*rgb[1] + 0.072169f*rgb[2];
        xyz[2] = 0.019334f*rgb[0] + 0.119193f*rgb[1] + 0.950227f*rgb[2];
    };

public:
    exrBool IsBlack() const;
    exrBool HasNaNs() const;

public:
    Spectrum GetSqrt() const;
    Spectrum GetExp() const;
    Spectrum GetPow(exrU32 e) const;
    Spectrum GetClamped(exrFloat low, exrFloat high) const;

protected:
    exrFloat m_Wavelengths[numSpectrumSamples];
};
exrEND_NAMESPACE