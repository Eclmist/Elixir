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