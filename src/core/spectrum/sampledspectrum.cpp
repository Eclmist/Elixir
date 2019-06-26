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

#include "sampledspectrum.h"

exrBEGIN_NAMESPACE

SampledSpectrum::SampledSpectrum(std::vector<exrFloat> wavelengths, std::vector<exrFloat> values)
{
    if (!SpectrumSamplesIsSorted(wavelengths))
    {
        SortSpectrumSamples(wavelengths, values);
    }

    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
    {
        exrFloat w0 = exrLerp(exrFloat(sampledWavelengthStart), exrFloat(sampledWavelengthEnd), exrFloat(i) / numSpectrumSamples);
        exrFloat w1 = exrLerp(exrFloat(sampledWavelengthStart), exrFloat(sampledWavelengthEnd), exrFloat(i + 1) / numSpectrumSamples);
        m_Wavelengths[i] = AverageSpectrumSamples(wavelengths, values, w0, w1);
    }
}

exrVector3 SampledSpectrum::ToXYZ() const
{
    exrVector3 xyz = exrVector3::Zero();
    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
    {
        xyz.x += m_X[i] * m_Wavelengths[i];
        xyz.y += m_Y[i] * m_Wavelengths[i];
        xyz.z += m_Z[i] * m_Wavelengths[i];
    }

    exrFloat scale = exrFloat(sampledWavelengthEnd - sampledWavelengthStart) / exrFloat(numSpectrumSamples);
    xyz *= scale;
}

exrFloat SampledSpectrum::GetLuminance() const
{
    // Luminance is usually closely related to the y coefficient of the XYZ color.
    // Therefore, as a good enough approximation we will just return the y coefficient
    exrFloat y = 0.0f;

    for (exrU32 i = 0; i < numSpectrumSamples; ++i)
        y += m_Y.m_Wavelengths[i] * m_Wavelengths[i];

    return y *= exrFloat(sampledWavelengthEnd - sampledWavelengthStart) / exrFloat(numSpectrumSamples);
}

void SampledSpectrum::Init()
{
    std::vector<exrFloat> CIE_Wavelengths(CIE_WavelengthsRaw, CIE_WavelengthsRaw + sizeof(CIE_WavelengthsRaw) / sizeof(CIE_WavelengthsRaw[0]));
    std::vector<exrFloat> CIE_XVal(CIE_X, CIE_X + sizeof(CIE_X) / sizeof(CIE_X[0]));
    std::vector<exrFloat> CIE_YVal(CIE_Y, CIE_Y + sizeof(CIE_Y) / sizeof(CIE_Y[0]));
    std::vector<exrFloat> CIE_ZVal(CIE_Z, CIE_Z + sizeof(CIE_Z) / sizeof(CIE_Z[0]));

    m_X = SampledSpectrum(CIE_Wavelengths, CIE_XVal);
    m_Y = SampledSpectrum(CIE_Wavelengths, CIE_YVal);
    m_Z = SampledSpectrum(CIE_Wavelengths, CIE_ZVal);
}

exrBool SampledSpectrum::SpectrumSamplesIsSorted(const std::vector<exrFloat>& wavelengths)
{
    const auto size = wavelengths.size() - 1;
    for (exrU32 i = 0; i < size; ++i)
        if (wavelengths[i] > wavelengths[i + 1]) return false;
    return true;
}

void SampledSpectrum::SortSpectrumSamples(std::vector<exrFloat>& wavelengths, std::vector<exrFloat>& values)
{
    std::vector<std::pair<exrFloat, exrFloat>> samples(wavelengths.size());

    for (exrU32 i = 0; i < wavelengths.size(); ++i)
        samples[i] = std::pair<exrFloat, exrFloat>(wavelengths[i], values[i]);

    std::sort(samples.begin(), samples.end(), [&](auto& left, auto& right)
    {
        return left.first > right.first;
    });

    for (exrU32 i = 0; i < wavelengths.size(); ++i)
    {
        wavelengths[i] = samples[i].first;
        values[i] = samples[i].second;
    }
}

exrFloat SampledSpectrum::AverageSpectrumSamples(const std::vector<exrFloat>& wavelengths, const std::vector<exrFloat>& values, exrFloat w0, exrFloat w1)
{
    if (w1 <= wavelengths[0]) return values[0]; // clamp left edge
    if (w0 >= wavelengths.back()) return values.back(); // clamp right edge
    if (wavelengths.size() == 1) return values[0]; // if only one value in SPD

    exrFloat sum = 0;

    // range to average goes beyond first and/or last supplied sample? clamp.
    if (w0 < wavelengths[0])
        sum += values[0] * (wavelengths[0] - w0);
    if (w1 > wavelengths.back())
        sum += values.back() * (w1 - wavelengths.back());

    // Find the first index where the starting wavelength of the interpolation range overlaps segments w0 to w1
    const auto size = wavelengths.size();
    exrU32 i = 0;
    while (w0 > wavelengths[i + 1]) ++i;

    // This following calculation is a little confusing, the process is described here
    // <http://www.pbr-book.org/3ed-2018/Color_and_Radiometry/The_SampledSpectrum_Class.html>
    auto interp = [wavelengths, values](exrFloat w, exrU32 i)
    {
        return exrLerp(values[i], values[i + 1], (w - wavelengths[i]) / (wavelengths[i + 1] - wavelengths[i]));
    };
    
    // iterate and compute average
    for (; i + 1 < size && w1 >= wavelengths[i]; ++i)
    {
        exrFloat segmentStart = exrMax(w0, wavelengths[i]);
        exrFloat segmentEnd = exrMin(w1, wavelengths[i + 1]);
        sum += 0.5f * (interp(segmentStart, i) + interp(segmentEnd, i)) * (segmentEnd - segmentStart);
    }

    return sum / (w1 - w0);
}

exrEND_NAMESPACE

