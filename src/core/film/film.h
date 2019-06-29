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

//! @brief A class writes the final image output of the renderer to a file
class Film
{
public:
    Film(const Point2<int>& resolution, const exrString& filename, exrBool stampFile = true)
        : m_Resolution(resolution)
        , m_FileName(filename)
        , m_StampFile(stampFile) {}

    void AddSplat(const exrPoint2& point, const exrVector3& value);
    void WriteImage(exrFloat splatScale);


private:
    struct Pixel
    {
        exrFloat m_FilterWeightSum = 0;
        AtomicFloat m_SplatRGB[3];
    };

    Pixel& GetPixel(const Point2<int>& point);

private:
    std::unique_ptr<Pixel[]> m_Pixels;

    Point2<int> m_Resolution;
    exrString m_FileName;
    exrBool m_StampFile;
};

exrEND_NAMESPACE