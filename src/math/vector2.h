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
class Vector2
{
public:
    Vector2<T>() : x(0), y(0) {}
    Vector2<T>(T _x, T _y) : x(_x), y(_y) {}
    Vector2<T>(T t) : x(t), y(t) {}
    Vector2<T>(const Vector2<T>& copy) : x(copy.x), y(copy.y) {}
    Vector2<T>(const T* const d) : x(d[0]), y(d[1]) {}

    inline const Vector2<T>& operator+() const { return *this; }
    inline Vector2<T> operator-() const { return Vector2<T>(-x, -y); }
    inline void operator=(const Vector2<T>& v) { x = v.x; y = v.y; }
    inline T operator[](int i) const { return m_Data[i]; }
    inline T& operator[](int i) { return m_Data[i]; }

    inline Vector2<T>& operator+=(const Vector2<T>& v) { x += v.x; y += v.y; return *this; }
    inline Vector2<T>& operator-=(const Vector2<T>& v) { x -= v.x; y -= v.y; return *this; }
    inline Vector2<T>& operator*=(const Vector2<T>& v) { x *= v.x; y *= v.y; return *this; }
    inline Vector2<T>& operator/=(const Vector2<T>& v) { x /= v.x; y /= v.y; return *this; }
    inline Vector2<T>& operator*=(const float t) { x *= t; y *= t; return *this; }
    inline Vector2<T>& operator/=(const float t) { x /= t; y /= t; return *this; }
    inline bool operator==(const Vector2<T>& v) { return (x == v.x) && (y == v.y); }

public:
    inline explicit operator Point<T>() const { return Point<T>(x, y, 0); }

public:
    inline const float SquareMagnitude() const { return x * x + y * y; }
    inline const float Magnitude() const { return sqrt(SquareMagnitude()); }

    inline Vector2<T> Normalized() const { return *this / Magnitude(); }

public:
    static Vector2<T> Zero() { return Vector2<T>(0.0f); }
    static Vector2<T> Up() { return Vector2<T>(0.0f, 1.0f); }
    static Vector2<T> Right() { return Vector2<T>(1.0f, 0.0f); }

public:
    union
    {
        struct { T x, y; };
        struct { T r, g; };
        struct { T m_Data[2]; };
    };
};

template<class T>
inline Vector2<T> operator+(const Vector2<T>& v1, const Vector2<T>& v2)
{
    return Vector2<T>(v1.x + v2.x, v1.y + v2.y);
}

template<class T>
inline Vector2<T> operator-(const Vector2<T>& v1, const Vector2<T>& v2)
{
    return Vector2<T>(v1.x - v2.x, v1.y - v2.y);
}

template<class T>
inline Vector2<T> operator*(const Vector2<T>& v1, const Vector2<T>& v2)
{
    return Vector2<T>(v1.x * v2.x, v1.y * v2.y);
}

template<class T>
inline Vector2<T> operator/(const Vector2<T>& v1, const Vector2<T>& v2)
{
    return Vector2<T>(v1.x / v2.x, v1.y / v2.y);
}

template<class T>
inline Vector2<T> operator*(const Vector2<T>& v, float t)
{
    return Vector2<T>(v.x * t, v.y * t);
}

template<class T>
inline Vector2<T> operator*(float t, const Vector2<T>& v)
{
    return Vector2<T>(v.x * t, v.y * t);
}

template<class T>
inline Vector2<T> operator/(const Vector2<T>& v, float t)
{
    return Vector2<T>(v.x / t, v.y / t);
}

template<class T>
inline float Dot(const Vector2<T>& v1, const Vector2<T>& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

template<class T>
inline Vector2<T> Reflect(const Vector2<T>& v, const Vector2<T>& n)
{
    return v - 2 * Dot(v, n) * n;
}

template<class T>
inline bool Refract(const Vector2<T>& v, const Vector2<T>& n, float ni_over_nt, Vector2<T>& refracted)
{
    Vector2<T> vNorm = v.Normalized();
    float dt = Dot(vNorm, n);
    float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);

    if (discriminant > 0)
    {
        refracted = ni_over_nt * (vNorm - n * dt) - n * sqrt(discriminant);
        return true;
    }

    return false;
}

exrEND_NAMESPACE