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

static SampledSpectrum m_X, m_Y, m_Z;
static SampledSpectrum m_rgbIllum2SpectWhite;
static SampledSpectrum m_rgbIllum2SpectCyan, m_rgbIllum2SpectMagenta, m_rgbIllum2SpectYellow;
static SampledSpectrum m_rgbIllum2SpectRed, m_rgbIllum2SpectGreen, m_rgbIllum2SpectBlue;
static SampledSpectrum m_rgbRefl2SpectWhite;
static SampledSpectrum m_rgbRefl2SpectCyan, m_rgbRefl2SpectMagenta, m_rgbRefl2SpectYellow;
static SampledSpectrum m_rgbRefl2SpectRed, m_rgbRefl2SpectGreen, m_rgbRefl2SpectBlue;

exrVector3 SampledSpectrum::ToXYZ() const
{
    exrVector3 xyz = exrVector3::Zero();
    for (int i = 0; i < numSpectrumSamples; ++i)
    {
        xyz.x += m_X[i] * m_Wavelengths[i];
        xyz.y += m_Y[i] * m_Wavelengths[i];
        xyz.z += m_Z[i] * m_Wavelengths[i];
    }

    exrFloat scale = exrFloat(sampledWavelengthEnd - sampledWavelengthStart) / exrFloat(numSpectrumSamples);
    xyz *= scale;
    return xyz;
}

exrVector3 SampledSpectrum::ToRGB() const
{
    exrVector3 xyz = ToXYZ();
    exrVector3 rgb;
    XYZToRGB(xyz.m_Data, rgb.m_Data);
    return rgb;
}

exrFloat SampledSpectrum::GetLuminance() const
{
    // Luminance is usually closely related to the y coefficient of the XYZ color.
    // Therefore, as a good enough approximation we will just return the y coefficient
    exrFloat y = 0.0f;

    for (int i = 0; i < numSpectrumSamples; ++i)
        y += m_Y.m_Wavelengths[i] * m_Wavelengths[i];

    return y *= exrFloat(sampledWavelengthEnd - sampledWavelengthStart) / exrFloat(numSpectrumSamples);
}

SampledSpectrum SampledSpectrum::FromRGB(const exrVector3& rgb, SpectrumType type)
{
    SampledSpectrum res;

    switch (type)
    {
    case SpectrumType::llluminance:
        if (rgb.r <= rgb.b && rgb.r <= rgb.g)
        {
            res += m_rgbIllum2SpectWhite * rgb.r;
            if (rgb.g <= rgb.b)
            {
                res += m_rgbIllum2SpectCyan * (rgb.g - rgb.r);
                res += m_rgbIllum2SpectBlue * (rgb.b - rgb.g);
            }
            else
            {
                res += m_rgbIllum2SpectCyan * (rgb.b - rgb.r);
                res += m_rgbIllum2SpectGreen * (rgb.g - rgb.b);
            }
        }
        else if (rgb.g <= rgb.r && rgb.g <= rgb.b)
        {
            res += m_rgbIllum2SpectWhite * rgb.g;
            if (rgb.r <= rgb.b)
            {
                res += m_rgbIllum2SpectMagenta * (rgb.r - rgb.g);
                res += m_rgbIllum2SpectBlue * (rgb.b - rgb.r);
            }
            else
            {
                res += m_rgbIllum2SpectMagenta * (rgb.b - rgb.g);
                res += m_rgbIllum2SpectRed * (rgb.r - rgb.b);
            }
        }
        else
        {
            res += m_rgbIllum2SpectWhite * rgb.b;
            if (rgb.r <= rgb.g)
            {
                res += m_rgbIllum2SpectYellow * (rgb.r - rgb.b);
                res += m_rgbIllum2SpectGreen * (rgb.g - rgb.r);
            }
            else
            {
                res += m_rgbIllum2SpectYellow * (rgb.g - rgb.b);
                res += m_rgbIllum2SpectRed * (rgb.r - rgb.g);
            }
        }

        break;
    case SpectrumType::Reflectance:
        if (rgb.r <= rgb.b && rgb.r <= rgb.g)
        {
            res += m_rgbRefl2SpectWhite * rgb.r;
            if (rgb.g <= rgb.b)
            {
                res += m_rgbRefl2SpectCyan * (rgb.g - rgb.r);
                res += m_rgbRefl2SpectBlue * (rgb.b - rgb.g);
            }
            else
            {
                res += m_rgbRefl2SpectCyan * (rgb.b - rgb.r);
                res += m_rgbRefl2SpectGreen * (rgb.g - rgb.b);
            }
        }
        else if (rgb.g <= rgb.r && rgb.g <= rgb.b)
        {
            res += m_rgbRefl2SpectWhite * rgb.g;
            if (rgb.r <= rgb.b)
            {
                res += m_rgbRefl2SpectMagenta * (rgb.r - rgb.g);
                res += m_rgbRefl2SpectBlue * (rgb.b - rgb.r);
            }
            else
            {
                res += m_rgbRefl2SpectMagenta * (rgb.b - rgb.g);
                res += m_rgbRefl2SpectRed * (rgb.r - rgb.b);
            }
        }
        else
        {
            res += m_rgbRefl2SpectWhite * rgb.b;
            if (rgb.r <= rgb.g)
            {
                res += m_rgbRefl2SpectYellow * (rgb.r - rgb.b);
                res += m_rgbRefl2SpectGreen * (rgb.g - rgb.r);
            }
            else
            {
                res += m_rgbRefl2SpectYellow * (rgb.g - rgb.b);
                res += m_rgbRefl2SpectRed * (rgb.r - rgb.g);
            }
        }
        break;
    default:
        throw "Invalid Spectrum Type!";
    }

    return res;
}

SampledSpectrum SampledSpectrum::FromXYZ(const exrVector3& xyz, SpectrumType type)
{
    exrVector3 rgb;
    XYZToRGB(xyz.m_Data, rgb.m_Data);
    return FromRGB(rgb, type);
}

SampledSpectrum SampledSpectrum::FromSampled(const std::vector<exrFloat>& wavelengths, const std::vector<exrFloat>& values)
{
    SampledSpectrum s;

    if (!SpectrumSamplesIsSorted(wavelengths))
    {
        std::vector<exrFloat> sortedWavelengths = wavelengths;
        std::vector<exrFloat> sortedValues = values;
        SortSpectrumSamples(sortedWavelengths, sortedValues);
        return FromSampled(sortedWavelengths, sortedValues);
    }

    for (int i = 0; i < numSpectrumSamples; ++i)
    {
        exrFloat w0 = exrLerp(exrFloat(sampledWavelengthStart), exrFloat(sampledWavelengthEnd), exrFloat(i) / numSpectrumSamples);
        exrFloat w1 = exrLerp(exrFloat(sampledWavelengthStart), exrFloat(sampledWavelengthEnd), exrFloat(i + 1) / numSpectrumSamples);
        s.m_Wavelengths[i] = AverageSpectrumSamples(wavelengths, values, w0, w1);
    }

    return s;
}

void SampledSpectrum::Init()
{
    std::vector<exrFloat> CIE_Wavelengths(CIE_WavelengthsRaw, CIE_WavelengthsRaw + sizeof(CIE_WavelengthsRaw) / sizeof(CIE_WavelengthsRaw[0]));
    std::vector<exrFloat> CIE_XVal(CIE_X, CIE_X + sizeof(CIE_X) / sizeof(CIE_X[0]));
    std::vector<exrFloat> CIE_YVal(CIE_Y, CIE_Y + sizeof(CIE_Y) / sizeof(CIE_Y[0]));
    std::vector<exrFloat> CIE_ZVal(CIE_Z, CIE_Z + sizeof(CIE_Z) / sizeof(CIE_Z[0]));

    m_X = SampledSpectrum::FromSampled(CIE_Wavelengths, CIE_XVal);
    m_Y = SampledSpectrum::FromSampled(CIE_Wavelengths, CIE_YVal);
    m_Z = SampledSpectrum::FromSampled(CIE_Wavelengths, CIE_ZVal);

    // Compute RGB to spectrum functions
    std::vector<exrFloat> RGB2SpectWavelengths(RGB2SpectWavelengthsRaw, RGB2SpectWavelengthsRaw + sizeof(RGB2SpectWavelengthsRaw) / sizeof(RGB2SpectWavelengthsRaw[0]));
    std::vector<exrFloat> RGBIllum2SpectWhiteVal(RGBIllum2SpectWhite, RGBIllum2SpectWhite + sizeof(RGBIllum2SpectWhite) / sizeof(RGBIllum2SpectWhite[0]));
    std::vector<exrFloat> RGBIllum2SpectCyanVal(RGBIllum2SpectCyan, RGBIllum2SpectCyan + sizeof(RGBIllum2SpectCyan) / sizeof(RGBIllum2SpectCyan[0]));
    std::vector<exrFloat> RGBIllum2SpectMagentaVal(RGBIllum2SpectMagenta, RGBIllum2SpectMagenta + sizeof(RGBIllum2SpectMagenta) / sizeof(RGBIllum2SpectMagenta[0]));
    std::vector<exrFloat> RGBIllum2SpectYellowVal(RGBIllum2SpectYellow, RGBIllum2SpectYellow + sizeof(RGBIllum2SpectYellow) / sizeof(RGBIllum2SpectYellow[0]));
    std::vector<exrFloat> RGBIllum2SpectRedVal(RGBIllum2SpectRed, RGBIllum2SpectRed + sizeof(RGBIllum2SpectRed) / sizeof(RGBIllum2SpectRed[0]));
    std::vector<exrFloat> RGBIllum2SpectGreenVal(RGBIllum2SpectGreen, RGBIllum2SpectGreen + sizeof(RGBIllum2SpectGreen) / sizeof(RGBIllum2SpectGreen[0]));
    std::vector<exrFloat> RGBIllum2SpectBlueVal(RGBIllum2SpectBlue, RGBIllum2SpectBlue + sizeof(RGBIllum2SpectBlue) / sizeof(RGBIllum2SpectBlue[0]));

    std::vector<exrFloat> RGBRefl2SpectWhiteVal(RGBRefl2SpectWhite, RGBRefl2SpectWhite + sizeof(RGBRefl2SpectWhite) / sizeof(RGBRefl2SpectWhite[0]));
    std::vector<exrFloat> RGBRefl2SpectCyanVal(RGBRefl2SpectCyan, RGBRefl2SpectCyan + sizeof(RGBRefl2SpectCyan) / sizeof(RGBRefl2SpectCyan[0]));
    std::vector<exrFloat> RGBRefl2SpectMagentaVal(RGBRefl2SpectMagenta, RGBRefl2SpectMagenta + sizeof(RGBRefl2SpectMagenta) / sizeof(RGBRefl2SpectMagenta[0]));
    std::vector<exrFloat> RGBRefl2SpectYellowVal(RGBRefl2SpectYellow, RGBRefl2SpectYellow + sizeof(RGBRefl2SpectYellow) / sizeof(RGBRefl2SpectYellow[0]));
    std::vector<exrFloat> RGBRefl2SpectRedVal(RGBRefl2SpectRed, RGBRefl2SpectRed + sizeof(RGBRefl2SpectRed) / sizeof(RGBRefl2SpectRed[0]));
    std::vector<exrFloat> RGBRefl2SpectGreenVal(RGBRefl2SpectGreen, RGBRefl2SpectGreen + sizeof(RGBRefl2SpectGreen) / sizeof(RGBRefl2SpectGreen[0]));
    std::vector<exrFloat> RGBRefl2SpectBlueVal(RGBRefl2SpectBlue, RGBRefl2SpectBlue + sizeof(RGBRefl2SpectBlue) / sizeof(RGBRefl2SpectBlue[0]));

    m_rgbIllum2SpectWhite   = SampledSpectrum::FromSampled(RGB2SpectWavelengths, RGBIllum2SpectWhiteVal);
    m_rgbIllum2SpectCyan    = SampledSpectrum::FromSampled(RGB2SpectWavelengths, RGBIllum2SpectCyanVal);
    m_rgbIllum2SpectMagenta = SampledSpectrum::FromSampled(RGB2SpectWavelengths, RGBIllum2SpectMagentaVal);
    m_rgbIllum2SpectYellow  = SampledSpectrum::FromSampled(RGB2SpectWavelengths, RGBIllum2SpectYellowVal);
    m_rgbIllum2SpectRed     = SampledSpectrum::FromSampled(RGB2SpectWavelengths, RGBIllum2SpectRedVal);
    m_rgbIllum2SpectGreen   = SampledSpectrum::FromSampled(RGB2SpectWavelengths, RGBIllum2SpectGreenVal);
    m_rgbIllum2SpectBlue    = SampledSpectrum::FromSampled(RGB2SpectWavelengths, RGBIllum2SpectBlueVal);

    m_rgbRefl2SpectWhite    = SampledSpectrum::FromSampled(RGB2SpectWavelengths, RGBRefl2SpectWhiteVal);
    m_rgbRefl2SpectCyan     = SampledSpectrum::FromSampled(RGB2SpectWavelengths, RGBRefl2SpectCyanVal);
    m_rgbRefl2SpectMagenta  = SampledSpectrum::FromSampled(RGB2SpectWavelengths, RGBRefl2SpectMagentaVal);
    m_rgbRefl2SpectYellow   = SampledSpectrum::FromSampled(RGB2SpectWavelengths, RGBRefl2SpectYellowVal);
    m_rgbRefl2SpectRed      = SampledSpectrum::FromSampled(RGB2SpectWavelengths, RGBRefl2SpectRedVal);
    m_rgbRefl2SpectGreen    = SampledSpectrum::FromSampled(RGB2SpectWavelengths, RGBRefl2SpectGreenVal);
    m_rgbRefl2SpectBlue     = SampledSpectrum::FromSampled(RGB2SpectWavelengths, RGBRefl2SpectBlueVal);
}

exrBool SampledSpectrum::SpectrumSamplesIsSorted(const std::vector<exrFloat>& wavelengths)
{
    const auto size = wavelengths.size() - 1;
    for (int i = 0; i < size; ++i)
        if (wavelengths[i] > wavelengths[i + 1]) return false;
    return true;
}

void SampledSpectrum::SortSpectrumSamples(std::vector<exrFloat>& wavelengths, std::vector<exrFloat>& values)
{
    std::vector<std::pair<exrFloat, exrFloat>> samples(wavelengths.size());

    for (int i = 0; i < wavelengths.size(); ++i)
        samples[i] = std::pair<exrFloat, exrFloat>(wavelengths[i], values[i]);

    std::sort(samples.begin(), samples.end(), [&](auto& left, auto& right)
    {
        return left.first > right.first;
    });

    for (int i = 0; i < wavelengths.size(); ++i)
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
    int i = 0;
    while (w0 > wavelengths[i + 1]) ++i;

    // This following calculation is a little confusing, the process is described here
    // <http://www.pbr-book.org/3ed-2018/Color_and_Radiometry/The_SampledSpectrum_Class.html>
    auto interp = [wavelengths, values](exrFloat w, int i)
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

