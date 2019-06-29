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

#include "math.h"

exrBEGIN_NAMESPACE

inline exrPoint3 operator*(const Matrix4x4& m, const exrPoint3& p)
{
    exrFloat x = p.x * m.m_Data[0] + p.y * m.m_Data[1] + p.z * m.m_Data[2] + m.m_Data[3];
    exrFloat y = p.x * m.m_Data[4] + p.y * m.m_Data[5] + p.z * m.m_Data[6] + m.m_Data[7];
    exrFloat z = p.x * m.m_Data[8] + p.y * m.m_Data[9] + p.z * m.m_Data[10] + m.m_Data[11];
    exrFloat w = p.x * m.m_Data[12] + p.y * m.m_Data[13] + p.z * m.m_Data[14] + m.m_Data[15];

    // if w is one, just return the point
    if (w == 1.0f)
        return exrPoint3(x, y, z);

    return exrPoint3(x, y, z) / w;
}

inline exrPoint3 operator*(const exrPoint3& p, const Matrix4x4& m)
{
    return m * p;
}

inline exrVector3 operator*(const Matrix4x4& m, const exrVector3& v)
{
    exrFloat _x = v.x * m.m_Data[0] + v.y * m.m_Data[1] + v.z * m.m_Data[2];
    exrFloat _y = v.x * m.m_Data[4] + v.y * m.m_Data[5] + v.z * m.m_Data[6];
    exrFloat _z = v.x * m.m_Data[8] + v.y * m.m_Data[9] + v.z * m.m_Data[10];
    // return the result
    return exrVector3(_x, _y, _z);
}

inline exrVector3 operator*(const exrVector3& v, const Matrix4x4& m)
{
    return m * v;
}

template<class T>
inline Point3<T> operator+(const Point3<T>& p, const Vector3<T>& v)
{
    return Point3<T>(p.x + v.x, p.y + v.y, p.z + v.z);
}

template<class T>
inline Point3<T> operator-(const Point3<T>& p, const Vector3<T>& v)
{
    return Point3<T>(p.x - v.x, p.y - v.y, p.z - v.z);
}

template<class T>
inline Vector3<T> operator-(const Point3<T>& p1, const Point3<T>& p2)
{
    return Vector3<T>(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

exrEND_NAMESPACE
