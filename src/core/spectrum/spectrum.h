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
    exrBool IsBlack() const;
    exrBool HasNaNs() const;

public:
    Spectrum GetSqrt() const;
    Spectrum GetExp() const;
    Spectrum GetPow(exrU32 e) const;
    Spectrum GetClamped(exrFloat low, exrFloat high) const;

    virtual exrFloat GetLuminance() const;

protected:
    exrFloat m_Wavelengths[numSpectrumSamples];
};

template <exrU32 N>
Spectrum<N>::Spectrum(exrFloat v)
{
    for (exrU32 i = 0; i < N; ++i)
    {
        m_Wavelengths[i] = v;
    }
}

template <exrU32 N>
Spectrum<N>& Spectrum<N>::operator+=(const Spectrum<N>& s2)
{
    for (exrU32 i = 0; i < N; ++i)
    {
        m_Wavelengths[i] += s2.m_Wavelengths[i];
    }

    return *this;
}

template <exrU32 N>
Spectrum<N>& Spectrum<N>::operator-=(const Spectrum<N>& s2)
{
    for (exrU32 i = 0; i < N; ++i)
    {
        m_Wavelengths[i] -= s2.m_Wavelengths[i];
    }

    return *this;
}

template <exrU32 N>
Spectrum<N>& Spectrum<N>::operator*=(const Spectrum<N>& s2)
{
    for (exrU32 i = 0; i < N; ++i)
    {
        m_Wavelengths[i] *= s2.m_Wavelengths[i];
    }

    return *this;
}

template <exrU32 N>
Spectrum<N>& Spectrum<N>::operator/=(const Spectrum<N>& s2)
{
    for (exrU32 i = 0; i < N; ++i)
    {
        m_Wavelengths[i] /= s2.m_Wavelengths[i];
    }

    return *this;
}

template <exrU32 N>
Spectrum<N>& Spectrum<N>::operator+=(exrFloat v)
{
    for (exrU32 i = 0; i < N; ++i)
    {
        m_Wavelengths[i] += v;
    }

    return *this;
}

template <exrU32 N>
Spectrum<N>& Spectrum<N>::operator-=(exrFloat v)
{
    for (exrU32 i = 0; i < N; ++i)
    {
        m_Wavelengths[i] -= v;
    }

    return *this;
}

template <exrU32 N>
Spectrum<N>& Spectrum<N>::operator*=(exrFloat v)
{
    for (exrU32 i = 0; i < N; ++i)
    {
        m_Wavelengths[i] *= v;
    }

    return *this;
}

template <exrU32 N>
Spectrum<N>& Spectrum<N>::operator/=(exrFloat v)
{
    for (exrU32 i = 0; i < N; ++i)
    {
        m_Wavelengths[i] /= v;
    }

    return *this;
}

template <exrU32 N>
Spectrum<N> Spectrum<N>::operator+(const Spectrum<N>& s2) const
{
    Spectrum<N> ret = *this;

    for (exrU32 i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] += s2.m_Wavelengths[i];
    }

    return ret;
}

template <exrU32 N>
Spectrum<N> Spectrum<N>::operator-(const Spectrum<N>& s2) const
{
    Spectrum<N> ret = *this;

    for (exrU32 i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] -= s2.m_Wavelengths[i];
    }

    return ret;
}

template <exrU32 N>
Spectrum<N> Spectrum<N>::operator*(const Spectrum<N>& s2) const
{
    Spectrum<N> ret = *this;

    for (exrU32 i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] *= s2.m_Wavelengths[i];
    }

    return ret;
}

template <exrU32 N>
Spectrum<N> Spectrum<N>::operator/(const Spectrum<N>& s2) const
{
    Spectrum<N> ret = *this;

    for (exrU32 i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] /= s2.m_Wavelengths[i];
    }

    return ret;
}

template <exrU32 N>
Spectrum<N> Spectrum<N>::operator+(exrFloat v) const
{
    Spectrum<N> ret = *this;

    for (exrU32 i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] += v;
    }

    return ret;
}

template <exrU32 N>
Spectrum<N> Spectrum<N>::operator-(exrFloat v) const
{
    Spectrum<N> ret = *this;

    for (exrU32 i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] -= v;
    }

    return ret;
}

template <exrU32 N>
Spectrum<N> Spectrum<N>::operator*(exrFloat v) const
{
    Spectrum<N> ret = *this;

    for (exrU32 i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] *= v;
    }

    return ret;
}

template <exrU32 N>
Spectrum<N> Spectrum<N>::operator/(exrFloat v) const
{
    Spectrum<N> ret = *this;

    for (exrU32 i = 0; i < N; ++i)
    {
        ret.m_Wavelengths[i] /= v;
    }

    return ret;
}

template <exrU32 N>
exrBool Spectrum<N>::IsBlack() const
{
    for (exrU32 i = 0; i < N; ++i)
        if (m_Wavelengths[i] != 0.0f) return false;

    return true;
}

template <exrU32 N>
exrBool Spectrum<N>::HasNaNs() const
{
    using namespace std;

    for (exrU32 i = 0; i < N; ++i)
        if (isnan(m_Wavelengths[i])) return true;

    return false;
}

template <exrU32 N>
Spectrum<N> Spectrum<N>::GetSqrt() const
{
    Spectrum<N> ret;
    for (exrU32 i = 0; i < N; ++i)
        ret.m_Wavelengths[i] = sqrt(m_Wavelengths[i]);
    return ret;
}

template <exrU32 N>
Spectrum<N> Spectrum<N>::GetPow(exrU32 e) const
{
    Spectrum<N> ret;
    for (exrU32 i = 0; i < N; ++i)
        ret.m_Wavelengths[i] = pow(m_Wavelengths[i], e);
    return ret;
}

template <exrU32 N>
Spectrum<N> Spectrum<N>::GetExp() const
{
    Spectrum<N> ret;
    for (exrU32 i = 0; i < N; ++i)
        ret.m_Wavelengths[i] = exp(m_Wavelengths[i]);
    return ret;
}

template <exrU32 N>
Spectrum<N> Spectrum<N>::GetClamped(exrFloat low, exrFloat high) const
{
    Spectrum<N> ret;
    for (exrU32 i = 0; i < N; ++i)
        ret.m_Wavelengths[i] = exrClamp(m_Wavelengths[i], low, high);
    return ret;
}

template <exrU32 N>
exrFloat Spectrum<N>::GetLuminance() const
{
    exrAssert(false, "We don't have a good way to compute luminance for spectrum yet!");
    return 0.0f;
}

exrEND_NAMESPACE
