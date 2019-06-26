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

#include "spectrum.h"

exrBEGIN_NAMESPACE

Spectrum::Spectrum(exrFloat v)
{
    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
    {
        m_Wavelengths[i] = v;
    }
}

Spectrum& Spectrum::operator+=(const Spectrum& s2)
{
    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
    {
        m_Wavelengths[i] += s2.m_Wavelengths[i];
    }

    return *this;
}

Spectrum& Spectrum::operator-=(const Spectrum& s2)
{
    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
    {
        m_Wavelengths[i] -= s2.m_Wavelengths[i];
    }

    return *this;
}

Spectrum& Spectrum::operator*=(const Spectrum& s2)
{
    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
    {
        m_Wavelengths[i] *= s2.m_Wavelengths[i];
    }

    return *this;
}

Spectrum& Spectrum::operator/=(const Spectrum& s2)
{
    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
    {
        m_Wavelengths[i] /= s2.m_Wavelengths[i];
    }

    return *this;
}

Spectrum Spectrum::operator+(const Spectrum& s2) const
{
    Spectrum ret = *this;

    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
    {
        ret.m_Wavelengths[i] += s2.m_Wavelengths[i];
    }

    return ret;
}

Spectrum Spectrum::operator-(const Spectrum& s2) const
{
    Spectrum ret = *this;

    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
    {
        ret.m_Wavelengths[i] -= s2.m_Wavelengths[i];
    }

    return ret;
}

Spectrum Spectrum::operator*(const Spectrum& s2) const
{
    Spectrum ret = *this;

    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
    {
        ret.m_Wavelengths[i] *= s2.m_Wavelengths[i];
    }

    return ret;
}

Spectrum Spectrum::operator/(const Spectrum& s2) const
{
    Spectrum ret = *this;

    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
    {
        ret.m_Wavelengths[i] /= s2.m_Wavelengths[i];
    }

    return ret;
}

exrBool Spectrum::IsBlack() const
{
    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
        if (m_Wavelengths[i] != 0.0f) return false;

    return true;
}

exrBool Spectrum::HasNaNs() const
{
    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
        if (std::isnan(m_Wavelengths[i])) return true;

    return false;
}

Spectrum Spectrum::GetSqrt() const
{
    Spectrum ret;
    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
        ret.m_Wavelengths[i] = sqrt(m_Wavelengths[i]);
    return ret;
}

Spectrum Spectrum::GetPow(exrU32 e) const
{
    Spectrum ret;
    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
        ret.m_Wavelengths[i] = pow(m_Wavelengths[i], e);
    return ret;
}

Spectrum Spectrum::GetExp() const
{
    Spectrum ret;
    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
        ret.m_Wavelengths[i] = exp(m_Wavelengths[i]);
    return ret;
}
exrEND_NAMESPACE
