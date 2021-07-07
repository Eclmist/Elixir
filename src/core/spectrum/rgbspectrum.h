/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019-2020 Samuel Van Allen - All rights reserved.

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
    exrVector3 ToRGB() const;
    exrFloat GetLuminance() const override;

public:
    static RGBSpectrum FromRGB(const exrVector3& rgb);
};

exrEND_NAMESPACE