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

constexpr exrU32 sampledWavelengthStart = 400;
constexpr exrU32 sampledWavelengthEnd = 700;
constexpr exrU32 numSpectrumSamples = 60;

class SampledSpectrum : public Spectrum<numSpectrumSamples>
{
public:
    SampledSpectrum(exrFloat v = 0.0f) : Spectrum(v) {};
    SampledSpectrum(std::vector<exrFloat> wavelengths, std::vector<exrFloat> values);

public:
    static exrBool SpectrumSamplesIsSorted(const std::vector<exrFloat>& wavelengths);
    static void SortSpectrumSamples(std::vector<exrFloat>& wavelengths, std::vector<exrFloat>& values);

private:
    exrFloat AverageSpectrumSamples(const std::vector<exrFloat>& wavelengths, const std::vector<exrFloat>& values, exrFloat w0, exrFloat w1);
};

exrEND_NAMESPACE