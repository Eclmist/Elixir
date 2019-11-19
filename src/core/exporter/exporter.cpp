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

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "exporter.h"
#include "stb/stb_image.h"
#include "stb/stbi_image_write.h"

exrBEGIN_NAMESPACE

Exporter::Exporter(const Point2<exrU32>& resolution, const exrString& filename, exrBool stampFile)
    : m_Resolution(resolution)
    , m_FileName(filename)
    , m_StampFile(stampFile)
{
    m_Pixels = std::make_unique<Pixel[]>(m_Resolution.x * m_Resolution.y);
}

void Exporter::WritePixel(const Point2<exrU32>& point, const exrSpectrum& value)
{
    exrAssert(point.x >= 0 && point.x <= m_Resolution.x, "Attempting to write to outside image bounds!");
    exrAssert(point.y >= 0 && point.y <= m_Resolution.y, "Attempting to write to outside image bounds!");

    Pixel& pixel = GetPixel(point);
    exrVector3 rgb = value.ToRGB();
    
    for (exrU32 i = 0; i < 3; ++i)
        pixel.m_RGB[i].Add(rgb[i]);
}

void Exporter::WriteImage(exrFloat splatScale)
{
    exrProfile("Image File Export");
    // PPM Headers
    exrString header = "P6\n" + std::to_string(m_Resolution.x) + " " + std::to_string(m_Resolution.y) + "\n255\n";
    std::vector<exrByte> buffer(header.begin(), header.end());
    const auto headerOffset = buffer.size();

    buffer.resize(buffer.size() + m_Resolution.x * m_Resolution.y * sizeof(exrByte) * 3);
    memcpy(buffer.data(), header.c_str(), strlen(header.c_str()));

    for (exrU32 y = 0; y < m_Resolution.y; ++y)
    {
        for (exrU32 x = 0; x < m_Resolution.x; ++x)
        {
            Pixel& pixel = GetPixel(Point2<exrU32>(x, y));
            exrVector3 rgb(pixel.m_RGB[0], pixel.m_RGB[1], pixel.m_RGB[2]);

            exrFloat r = exrSaturate(rgb.r * splatScale);
            exrFloat g = exrSaturate(rgb.g * splatScale);
            exrFloat b = exrSaturate(rgb.b * splatScale);
            
            // Correct gamma and to 0 - 256 (exclusive) range
            // 256 because PPM takes ranges up to 256 exclusive
            r = sqrt(r) * 255.99f;
            g = sqrt(g) * 255.99f;
            b = sqrt(b) * 255.99f;

            const auto offset = 3 * ((m_Resolution.y - 1 - y) * m_Resolution.x + x) + headerOffset;
            buffer[offset] = static_cast<exrByte>(r);
            buffer[offset + 1] = static_cast<exrByte>(g);
            buffer[offset + 2] = static_cast<exrByte>(b);
        }
    }

    // hard-coded write to PNG for now
    // TODO: template this
    exrS32 x, y, n;
    stbi_uc* ppmOut = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size()), &x, &y, &n, 0);
    exrString filename = m_FileName;
    if (m_StampFile)
        filename += "_" + std::to_string(Timer::TimeSinceEpochMillisec());
    filename += ".png";

    stbi_write_png(filename.c_str(), m_Resolution.x, m_Resolution.y, 3, ppmOut, m_Resolution.x * 3);

#ifdef EXR_PLATFORM_WIN
    system(filename.c_str());
#elif defined EXR_PLATFORM_MAC
    system(("open " + filename).c_str());
#endif
}

Exporter::Pixel& Exporter::GetPixel(const Point2<exrU32>& point)
{
    exrU32 offset = point.x + (point.y * m_Resolution.x);
    return m_Pixels[offset];
}

exrEND_NAMESPACE
