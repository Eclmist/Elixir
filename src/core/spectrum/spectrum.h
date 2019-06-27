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

static const int numRGB2SpectSamples = 32;
extern const exrFloat RGB2SpectWavelengthsRaw[numRGB2SpectSamples];
extern const exrFloat RGBRefl2SpectWhite[numRGB2SpectSamples];
extern const exrFloat RGBRefl2SpectCyan[numRGB2SpectSamples];
extern const exrFloat RGBRefl2SpectMagenta[numRGB2SpectSamples];
extern const exrFloat RGBRefl2SpectYellow[numRGB2SpectSamples];
extern const exrFloat RGBRefl2SpectRed[numRGB2SpectSamples];
extern const exrFloat RGBRefl2SpectGreen[numRGB2SpectSamples];
extern const exrFloat RGBRefl2SpectBlue[numRGB2SpectSamples];
extern const exrFloat RGBIllum2SpectWhite[numRGB2SpectSamples];
extern const exrFloat RGBIllum2SpectCyan[numRGB2SpectSamples];
extern const exrFloat RGBIllum2SpectMagenta[numRGB2SpectSamples];
extern const exrFloat RGBIllum2SpectYellow[numRGB2SpectSamples];
extern const exrFloat RGBIllum2SpectRed[numRGB2SpectSamples];
extern const exrFloat RGBIllum2SpectGreen[numRGB2SpectSamples];
extern const exrFloat RGBIllum2SpectBlue[numRGB2SpectSamples];

enum class SpectrumType
{ 
    Reflectance,
    llluminance
};

template <exrU32 numSpectrumSamples>
class Spectrum
{
public:
    Spectrum(exrFloat v = 0.0f);

    Spectrum& operator+=(const Spectrum& s2);
    Spectrum& operator-=(const Spectrum& s2);
    Spectrum& operator*=(const Spectrum& s2);
    Spectrum& operator/=(const Spectrum& s2);
    Spectrum& operator+=(exrFloat v);
    Spectrum& operator-=(exrFloat v);
    Spectrum& operator*=(exrFloat v);
    Spectrum& operator/=(exrFloat v);

    Spectrum operator+(const Spectrum& s2) const;
    Spectrum operator-(const Spectrum& s2) const;
    Spectrum operator*(const Spectrum& s2) const;
    Spectrum operator/(const Spectrum& s2) const;
    Spectrum operator+(exrFloat v) const;
    Spectrum operator-(exrFloat v) const;
    Spectrum operator*(exrFloat v) const;
    Spectrum operator/(exrFloat v) const;

    // This implies that the set of coefficients are linear
    inline exrFloat& operator[](exrU32 i) { return m_Wavelengths[i]; }

public:
    virtual exrVector3 ToXYZ() const;
    virtual exrVector3 ToRGB() const;

public:
    // Conversion to RGB based on a standard set of RGB spectra that has been
    // defined for high-definition televisions
    static void XYZToRGB(const exrFloat xyz[3], exrFloat rgb[3]);

    // The inverse of the xyz to rgb conversion
    static void RGBToXYZ(const exrFloat rgb[3], exrFloat xyz[3]);

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