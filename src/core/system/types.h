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

template <class T> class Vector3;
template <class T> class Vector2;
template <class T> class Point;
class Ray;
class Matrix4x4;

typedef std::string             exrString;
typedef Vector2<float>          exrVector2;
typedef Vector3<float>          exrVector3;
typedef Point<float>            exrPoint;
typedef bool                    exrBool;
typedef float                   exrFloat;
typedef float                   exrF32;
typedef double                  exrF64;
typedef unsigned char           exrByte;
typedef unsigned char           exrChar;
typedef unsigned short          exrU16;
typedef short                   exrS16;
typedef unsigned int            exrU32;
typedef int                     exrS32;
typedef unsigned long           exrU64;
typedef long                    exrS64;

exrEND_NAMESPACE
