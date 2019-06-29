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

#include "system/system.h"
#include "math/math.h"
#include "math/conversionutils.h"

exrBEGIN_NAMESPACE

static constexpr int numCIESamples = 471;
extern const exrFloat CIE_X[numCIESamples];
extern const exrFloat CIE_Y[numCIESamples];
extern const exrFloat CIE_Z[numCIESamples];
extern const exrFloat CIE_WavelengthsRaw[numCIESamples];

static constexpr int numRGB2SpectSamples = 32;
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

template <int numSpectrumSamples>
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
    inline exrFloat& operator[](int i) { return m_Wavelengths[i]; }

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
    Spectrum GetPow(int e) const;
    Spectrum GetClamped(exrFloat low, exrFloat high) const;

protected:
    exrFloat m_Wavelengths[numSpectrumSamples];
};

template <int N>
Spectrum<N>::Spectrum(exrFloat v)
{
    for (int i = 0; i < N; ++i)
    {
        m_Wavelengths[i] = v;
    }
}

template <int N>
Spectrum<N>& Spectrum<N>::operator+=(const Spectrum<N>& s2)
{
    for (int i = 0; i < N; ++i)
    {
        m_Wavelengths[i] += s2.m_Wavelengths[i];
    }

    return *this;
}

template <int N>
Spectrum<N>& Spectrum<N>::operator-=(const Spectrum<N>& s2)
{
    for (int i = 0; i < N; ++i)
    {
        m_Wavelengths[i] -= s2.m_Wavelengths[i];
    }

    return *this;
}

template <int N>
Spectrum<N>& Spectrum<N>::operator*=(const Spectrum<N>& s2)
{
    for (int i = 0; i < N; ++i)
    {
        m_Wavelengths[i] *= s2.m_Wavelengths[i];
    }

    return *this;
}

template <int N>
Spectrum<N>& Spectrum<N>::operator/=(const Spectrum<N>& s2)
{
    for (int i = 0; i < N; ++i)
    {
        m_Wavelengths[i] /= s2.m_Wavelengths[i];
    }

    return *this;
}

template <int N>
Spectrum<N>& Spectrum<N>::operator+=(exrFloat v)
{
    for (int i = 0; i < N; ++i)
    {
        m_Wavelengths[i] += v;
    }

    return *this;
}

template <int N>
Spectrum<N>& Spectrum<N>::operator-=(exrFloat v)
{
    for (int i = 0; i < N; ++i)
    {
        m_Wavelengths[i] -= v;
    }

    return *this;
}

template <int N>
Spectrum<N>& Spectrum<N>::operator*=(exrFloat v)
{
    for (int i = 0; i < N; ++i)
    {
        m_Wavelengths[i] *= v;
    }

    return *this;
}

template <int N>
Spectrum<N>& Spectrum<N>::operator/=(exrFloat v)
{
    for (int i = 0; i < N; ++i)
    {
        m_Wavelengths[i] /= v;
    }

    return *this;
}

template <int N>
Spectrum<N> Spectrum<N>::operator+(const Spectrum<N>& s2) const
{
    Spectrum<N> ret = *this;

    for (int i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] += s2.m_Wavelengths[i];
    }

    return ret;
}

template <int N>
Spectrum<N> Spectrum<N>::operator-(const Spectrum<N>& s2) const
{
    Spectrum<N> ret = *this;

    for (int i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] -= s2.m_Wavelengths[i];
    }

    return ret;
}

template <int N>
Spectrum<N> Spectrum<N>::operator*(const Spectrum<N>& s2) const
{
    Spectrum<N> ret = *this;

    for (int i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] *= s2.m_Wavelengths[i];
    }

    return ret;
}

template <int N>
Spectrum<N> Spectrum<N>::operator/(const Spectrum<N>& s2) const
{
    Spectrum<N> ret = *this;

    for (int i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] /= s2.m_Wavelengths[i];
    }

    return ret;
}

template <int N>
Spectrum<N> Spectrum<N>::operator+(exrFloat v) const
{
    Spectrum<N> ret = *this;

    for (int i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] += v;
    }

    return ret;
}

template <int N>
Spectrum<N> Spectrum<N>::operator-(exrFloat v) const
{
    Spectrum<N> ret = *this;

    for (int i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] -= v;
    }

    return ret;
}

template <int N>
Spectrum<N> Spectrum<N>::operator*(exrFloat v) const
{
    Spectrum<N> ret = *this;

    for (int i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] *= v;
    }

    return ret;
}

template <int N>
Spectrum<N> Spectrum<N>::operator/(exrFloat v) const
{
    Spectrum<N> ret = *this;

    for (int i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] /= v;
    }

    return ret;
}

template <int N>
void Spectrum<N>::XYZToRGB(const exrFloat xyz[3], exrFloat rgb[3])
{
    rgb[0] = 3.240479f*xyz[0] - 1.537150f*xyz[1] - 0.498535f*xyz[2];
    rgb[1] = -0.969256f*xyz[0] + 1.875991f*xyz[1] + 0.041556f*xyz[2];
    rgb[2] = 0.055648f*xyz[0] - 0.204043f*xyz[1] + 1.057311f*xyz[2];
}

template <int N>
void Spectrum<N>::RGBToXYZ(const exrFloat rgb[3], exrFloat xyz[3])
{
    xyz[0] = 0.412453f*rgb[0] + 0.357580f*rgb[1] + 0.180423f*rgb[2];
    xyz[1] = 0.212671f*rgb[0] + 0.715160f*rgb[1] + 0.072169f*rgb[2];
    xyz[2] = 0.019334f*rgb[0] + 0.119193f*rgb[1] + 0.950227f*rgb[2];
}

template <int N>
exrBool Spectrum<N>::IsBlack() const
{
    for (int i = 0; i < N; ++i)
        if (m_Wavelengths[i] != 0.0f) return false;

    return true;
}

template <int N>
exrBool Spectrum<N>::HasNaNs() const
{
    for (int i = 0; i < N; ++i)
        if (isnan(m_Wavelengths[i])) return true;

    return false;
}

template <int N>
Spectrum<N> Spectrum<N>::GetSqrt() const
{
    Spectrum<N> ret;
    for (int i = 0; i < N; ++i)
        ret.m_Wavelengths[i] = sqrt(m_Wavelengths[i]);
    return ret;
}

template <int N>
Spectrum<N> Spectrum<N>::GetPow(int e) const
{
    Spectrum<N> ret;
    for (int i = 0; i < N; ++i)
        ret.m_Wavelengths[i] = pow(m_Wavelengths[i], e);
    return ret;
}

template <int N>
Spectrum<N> Spectrum<N>::GetExp() const
{
    Spectrum<N> ret;
    for (int i = 0; i < N; ++i)
        ret.m_Wavelengths[i] = exp(m_Wavelengths[i]);
    return ret;
}

exrEND_NAMESPACE
