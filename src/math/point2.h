/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019-2020 Samuel Van Allen - All rights reserved.

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

#include "math/mathutils.h"

exrBEGIN_NAMESPACE

template <class T>
class Point2
{
public:
    Point2<T>() : x(0), y(0) {}
    Point2<T>(T _x, T _y) : x(_x), y(_y) {}
    Point2<T>(T t) : x(t), y(t) {}
    Point2<T>(const Point2<T>& copy) : x(copy.x), y(copy.y) {}
    Point2<T>(const T* const d) : x(d[0]), y(d[1]) {}

    inline const Point2<T>& operator+() const { return *this; }
    inline Point2<T> operator-() const { return Point2<T>(-x, -y); }
    inline void operator=(const Point2<T>& v) { x = v.x; y = v.y; }
    inline T operator[](int i) const { return m_Data[i]; }
    inline T& operator[](int i) { return m_Data[i]; }

    inline Point2<T>& operator+=(const Vector2<T>& v) { x += v.x; y += v.y; return *this; }
    inline Point2<T>& operator-=(const Vector2<T>& v) { x -= v.x; y -= v.y; return *this; }
    inline bool operator==(const Point2<T>& p) { return (x == p.x) && (y == p.y); }

public:
    inline explicit operator Vector2<T>() const { return Vector2<T>(x, y); }

public:
    static Point2<T> Zero() { return Point2<T>(0.0f); }

public:
    union
    {
        struct { T x, y; };
        struct { T m_Data[2]; };
    };
};

template<class T>
inline Point2<T> operator+(const Point2<T>& p, const Vector2<T>& v)
{
    return Point2<T>(p.x + v.x, p.y + v.y);
}

template<class T>
inline Point2<T> operator-(const Point2<T>& p, const Vector2<T>& v)
{
    return Point2<T>(p.x - v.x, p.y - v.y);
}

template<class T>
inline Point2<T> operator-(const Point2<T>& p1, const Point2<T>& p2)
{
    return Point2<T>(p1.x - p2.x, p1.y - p2.y);
}

template<class T>
inline Point2<T> operator*(const Point2<T>& p, const float f)
{
    return Point2<T>(p.x * f, p.y * f);
}

template<class T>
inline Point2<T> operator*(const float f, const Point2<T>& p)
{
    return Point2<T>(p.x * f, p.y * f);
}

template<class T>
inline float DistanceSquared(const Point2<T>& p1, const Point2<T>& p2)
{
    return (p2 - p1).SquareMagnitude();
}

template<class T>
inline float Distance(const Point2<T>& p1, const Point2<T>& p2)
{
    return (p2 - p1).Magnitude();
}

template<class T>
inline Point2<T> Min(const Point2<T>& p1, const Point2<T>& p2)
{
    return Point2<T>(exrMin(p1.x, p2.x), exrMin(p1.y, p2.y));
}

template<class T>
inline Point2<T> Max(const Point2<T>& p1, const Point2<T>& p2)
{
    return Point2<T>(exrMax(p1.x, p2.x), exrMax(p1.y, p2.y));
}

template<class T>
inline Point2<T> Floor(const Point2<T>& p)
{
    return Point2<T>(floor(p.x), floor(p.y));
}

template<class T>
inline Point2<T> Ceil(const Point2<T>& p)
{
    return Point2<T>(ceil(p.x), ceil(p.y));
}

template<class T>
inline Point2<T> Abs(const Point2<T>& p)
{
    return Point2<T>(abs(p.x), abs(p.y));
}
exrEND_NAMESPACE