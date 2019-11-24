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

#include "rgbspectrum.h"

exrBEGIN_NAMESPACE

exrVector3 RGBSpectrum::ToRGB() const
{
    exrVector3 rgb = exrVector3::Zero();
    rgb.r = m_Wavelengths[0];
    rgb.g = m_Wavelengths[1];
    rgb.b = m_Wavelengths[2];
    return rgb;
}

RGBSpectrum RGBSpectrum::FromRGB(const exrVector3& rgb)
{
    RGBSpectrum s;
    s.m_Wavelengths[0] = rgb.r;
    s.m_Wavelengths[1] = rgb.g;
    s.m_Wavelengths[2] = rgb.b;
    return s;
}

exrFloat RGBSpectrum::GetLuminance() const
{
    return 0.2126f * m_Wavelengths[0] +
           0.7152f * m_Wavelengths[1] + 
           0.0722f * m_Wavelengths[2];

}

exrEND_NAMESPACE

