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

#include <string>

exrBEGIN_NAMESPACE

class Matrix4x4;
class Ray;
class RGBSpectrum;
class SampledSpectrum;

template <class T> class Vector2;
template <class T> class Vector3;
template <class T> class Point2;
template <class T> class Point3;

typedef bool                    exrBool;
typedef unsigned char           exrByte;
typedef char                    exrChar;
typedef unsigned short          exrU16;
typedef short                   exrS16;
typedef float                   exrFloat;
typedef float                   exrF32;
typedef double                  exrF64;
typedef unsigned int            exrU32;
typedef int                     exrS32;
typedef unsigned long           exrU64;
typedef long                    exrS64;
typedef std::string             exrString;
typedef Vector2<exrF32>         exrVector2;
typedef Vector3<exrF32>         exrVector3;
typedef Point2<exrF32>          exrPoint2;
typedef Point3<exrF32>          exrPoint3;
typedef SampledSpectrum         exrSpectrum;

exrEND_NAMESPACE
