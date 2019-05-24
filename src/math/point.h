#pragma once

exrBEGIN_NAMESPACE

template <class T>
class Point
{
public:
    Point<T>() : x(0), y(0), z(0) {}
    Point<T>(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    Point<T>(T t) : x(t), y(t), z(t) {}
    Point<T>(const Point<T>& copy) : x(copy.x), y(copy.y), z(copy.z) {}
    Point<T>(const T* const d) : x(d[0]), y(d[1]), z(d[2]) {}

    inline const Point<T>& operator+() const { return *this; }
    inline Point<T> operator-() const { return Point<T>(-x, -y, -z); }
    inline Point<T> operator/(float w) const { return Point<T>(x / w, y / w, z / w); }
    inline T operator[](int i) const { return m_Data[i]; }
    inline T& operator[](int i) { return m_Data[i]; }

    inline Point<T>& operator+=(const Vector3<T>& v) { x += v.x; y += v.y; z += v.z; return *this; }
    inline Point<T>& operator-=(const Vector3<T>& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    inline bool operator==(const Point<T>& v) { return (x == v.x) && (y == v.y) && (z == v.z); }

public:
    static Point<T> Zero() { return Point<T>(0.0f); }

public:
    union
    {
        struct { T x, y, z; };
        struct { T m_Data[3]; };
    };
};

template<class T>
inline Point<T> operator+(const Point<T>& p, const Vector3<T>& v)
{
    return Point<T>(p.x + v.x, p.y + v.y, p.z + v.z);
}

template<class T>
inline Point<T> operator-(const Point<T>& p, const Vector3<T>& v)
{
    return Point<T>(p.x - v.x, p.y - v.y, p.z - v.z);
}

template<class T>
inline Vector3<T> operator-(const Point<T>& p1, const Point<T>& p2)
{
    return Vector3<T>(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

template<class T>
inline float DistanceSquared(const Point<T>& p1, const Point<T>& p2)
{
    return (p2 - p1).SquareMagnitude();
}

template<class T>
inline float Distance(const Point<T>& p1, const Point<T>& p2)
{
    return (p2 - p1).Magnitude();
}

exrEND_NAMESPACE