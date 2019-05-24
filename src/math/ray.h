#ifndef __MATH_RAY_H__
#define __MATH_RAY_H__

#include "core/elixir.h"

exrBEGIN_NAMESPACE

//! @brief A class that defines a mathematical ray
//!
//! Defines a ray with a origin, direction and distance in the parametric form of 
//! p(t) = origin + t * distance
class Ray
{
public:
    //! @brief Constructs a ray with an origin, direction and distance
    //! @param origin           The origin of the ray in world space
    //! @param direction        The normalized direction of the ray
    //! @param distance         The maximum ray distance
    Ray(const exrPoint& origin = exrPoint::Zero(), const exrVector3& direction = exrVector3::Zero(), exrFloat distance = 1000.0f)
        : m_Origin(origin), m_Direction(direction), m_Distance(distance) {};

    //! @brief Copy constructor. Constructs a ray with the same origin, direction and distance from input
    //! @param copy             The ray to copy
    Ray(const Ray& copy);
    
    //! @brief Returns the point along the ray at distance t
    //! 
    //! Evaluates the ray with t and return the point on the ray such that
    //! point = origin + t * direction
    //! 
    //! @param t                The t param in parametric ray equation
    //! 
    //! @return                 The point along the ray at distance t
    inline exrPoint operator()(exrFloat t) const { return m_Origin + t * m_Direction; }

public:
    //! The origin of the ray in world space
    exrPoint m_Origin;

    //! The normalized direction of the ray
    exrVector3 m_Direction;

    //! The maximum distance of the ray
    exrFloat m_Distance;
};

exrEND_NAMESPACE

#endif // !__MATH_RAY_H__

