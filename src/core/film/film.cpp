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

#include "film.h"
#include "stb/stb_image.h"
#include "stb/stbi_image_write.h"

exrBEGIN_NAMESPACE

void Film::AddSplat(const exrPoint2& point, const exrVector3& value)
{
    Pixel& pixel = GetPixel(Point2<int>((int)point.x, (int)point.y));

    for (int i = 0; i < 3; ++i)
        pixel.m_SplatRGB[i].Add(value[i]);
}

void Film::WriteImage(exrFloat splatScale)
{
    // PPM Headers
    exrString header = "P6\n" + std::to_string(m_Resolution.x) + " " + std::to_string(m_Resolution.y) + "\n255\n";
    std::vector<exrByte> buffer(header.begin(), header.end());
    const auto headerOffset = buffer.size();

    buffer.resize(buffer.size() + m_Resolution.x * m_Resolution.y * sizeof(exrByte) * 3);
    memcpy(buffer.data(), header.c_str(), strlen(header.c_str()));

    Point2<int> point;
    for (int y = 0; y < m_Resolution.y; ++y)
    {
        for (int x = 0; x < m_Resolution.x; ++x)
        {
            point.x = x;
            point.y = y;
            Pixel& pixel = GetPixel(point);

            exrFloat r = exrSaturate(pixel.m_SplatRGB[0] / splatScale);
            exrFloat g = exrSaturate(pixel.m_SplatRGB[1] / splatScale);
            exrFloat b = exrSaturate(pixel.m_SplatRGB[2] / splatScale);
            
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

    // hardcoded write to PNG for now
    // TODO: template this
    exrS32 x, y, n;
    stbi_uc* ppmOut = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size()), &x, &y, &n, 0);
    exrString filename = m_FileName;
    if (m_StampFile)
        filename += "_" + std::to_string(Timer::TimeSinceEpochMillisec());
    filename += ".png";

    stbi_write_png(filename.c_str(), m_Resolution.x, m_Resolution.y, 3, ppmOut, m_Resolution.x * 3);
}

Film::Pixel& Film::GetPixel(const Point2<int>& point)
{
    int offset = point.x + (point.y * m_Resolution.x);
    return m_Pixels[offset];
}

exrEND_NAMESPACE
