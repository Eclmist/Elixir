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

#include "spectrum.h"

exrBEGIN_NAMESPACE

class RGBSpectrum : public Spectrum<3>
{
public:
    RGBSpectrum(exrFloat v = 0.0f) : Spectrum(v) {};
    RGBSpectrum(const Spectrum<3>& v) : Spectrum(v) {};

public:
    // virtual functions
    exrVector3 ToXYZ() const override;
    exrVector3 ToRGB() const override;

public:
    // SpectrumType may not be used here, but since we may swap out which kind of spectrum
    // objects to use on compile time, we need to have the same signature.
    static RGBSpectrum FromRGB(const exrVector3& rgb, SpectrumType type);
    static RGBSpectrum FromXYZ(const exrVector3& xyz, SpectrumType type);
};

exrEND_NAMESPACE