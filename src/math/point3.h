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

template <class T>
class Point3
{
public:
    Point3<T>() : x(0), y(0), z(0) {}
    Point3<T>(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    Point3<T>(T t) : x(t), y(t), z(t) {}
    Point3<T>(const Point3<T>& copy) : x(copy.x), y(copy.y), z(copy.z) {}
    Point3<T>(const T* const d) : x(d[0]), y(d[1]), z(d[2]) {}

    inline const Point3<T>& operator+() const { return *this; }
    inline Point3<T> operator-() const { return Point3<T>(-x, -y, -z); }
    inline Point3<T> operator/(float w) const { return Point3<T>(x / w, y / w, z / w); }
    inline T operator[](int i) const { return m_Data[i]; }
    inline T& operator[](int i) { return m_Data[i]; }

    inline Point3<T>& operator+=(const Vector3<T>& v) { x += v.x; y += v.y; z += v.z; return *this; }
    inline Point3<T>& operator-=(const Vector3<T>& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    inline bool operator==(const Point3<T>& p) { return (x == p.x) && (y == p.y) && (z == p.z); }

public:
    inline explicit operator Vector3<T>() const { return Vector3<T>(x, y, z); }

public:
    static Point3<T> Zero() { return Point3<T>(0.0f); }

public:
    union
    {
        struct { T x, y, z; };
        struct { T m_Data[3]; };
    };
};

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

template<class T>
inline float DistanceSquared(const Point3<T>& p1, const Point3<T>& p2)
{
    return (p2 - p1).SquareMagnitude();
}

template<class T>
inline float Distance(const Point3<T>& p1, const Point3<T>& p2)
{
    return (p2 - p1).Magnitude();
}

exrEND_NAMESPACE