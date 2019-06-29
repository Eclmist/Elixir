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

static constexpr int sampledWavelengthStart = 400;
static constexpr int sampledWavelengthEnd = 700;
static constexpr int numSpectrumSamples = 60;

class SampledSpectrum : public Spectrum<numSpectrumSamples>
{
public:
    SampledSpectrum(exrFloat v = 0.0f) : Spectrum(v) {};
    exrFloat GetLuminance() const;

public:
    // virtual functions
    exrVector3 ToXYZ() const;
    exrVector3 ToRGB() const;

public:
    static SampledSpectrum FromRGB(const exrVector3& rgb, SpectrumType type);
    static SampledSpectrum FromXYZ(const exrVector3& xyz, SpectrumType type);
    static SampledSpectrum FromSampled(const std::vector<exrFloat>& wavelengths, const std::vector<exrFloat>& values);

    static void Init();
    static void SortSpectrumSamples(std::vector<exrFloat>& wavelengths, std::vector<exrFloat>& values);
    static exrBool SpectrumSamplesIsSorted(const std::vector<exrFloat>& wavelengths);
    static exrFloat AverageSpectrumSamples(const std::vector<exrFloat>& wavelengths, const std::vector<exrFloat>& values, exrFloat w0, exrFloat w1);
};

exrEND_NAMESPACE