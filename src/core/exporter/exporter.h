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
class Exporter
{
public:
    Exporter(const Point2<exrU32>& resolution, const exrString& filename, exrBool stampFile = true);

    // Warning: WritePixel is an ADDITIVE operation! 
    void WritePixel(const Point2<exrU32>& point, const exrSpectrum& value);
    void WriteErrorPixel(const Point2<exrU32>& point);
    void WriteImage(exrFloat splatScale);
    void FilterImage(exrU32 numIteration);

public:
    Point2<exrU32> m_Resolution;
    exrString m_FileName;

private:
    struct Pixel
    {
        AtomicFloat m_RGB[3];
    };

    Pixel& GetPixel(const Point2<exrU32>& point);

private:
    std::unique_ptr<Pixel[]> m_Pixels;
    exrBool m_StampFile;
};

exrEND_NAMESPACE