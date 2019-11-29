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

class Shape;

//! @brief A bounding volume that can be used in accelerators
//!
//! An axis aligned bounding volume that can be used to accelerate intersection test
//! with more complex geometry.
class AABB
{
public:
    //! @brief Constructor from two points
    //! @param min              The minimum extents of the bounding volume in world space
    //! @param max              The maximum extents of the bounding volume in world space
    AABB(const exrPoint3& min = exrPoint3(-1.0f), const exrPoint3& max = exrPoint3(1.0f));

    //! @brief Copy Constructor
    //! @param copy             The bounding volume to copy from
    AABB(const AABB& copy)
        : m_Min(copy.m_Min)
        , m_Max(copy.m_Max) {};

    //! @brief Returns minimum extents of the bounding volume in world space
    //! @return                 The minimum extends of the bounding volume
    inline exrPoint3 Min() const { return m_Min; }

    //! @brief Returns maximum extents of the bounding volume in world space
    //! @return                 The maximum extends of the bounding volume
    inline exrPoint3 Max() const { return m_Max; }

    //! @brief Returns the extents of the bounding volume in local space
    //! @return                 The extents of the bounding volume
    inline exrVector3 GetExtents() const { return m_Max - m_Min; }

    //! @brief Return the surface area of the bounding volume
    //! @return                 The surface area of the bounding volume
    inline exrFloat GetSurfaceArea() const { return (m_Max.x - m_Min.x) * (m_Max.y - m_Min.y) * 2 +
                                                    (m_Max.y - m_Min.y) * (m_Max.z - m_Min.z) * 2 +
                                                    (m_Max.x - m_Min.x) * (m_Max.z - m_Min.z) * 2; }

    //! @brief Sets the min extents of the bounding volume
    //! @param min              The minimum extends of the bounding volume
    inline void SetMin(exrPoint3 min) { m_Min = min; }

    //! @brief Sets the max extents of the bounding volume
    //! @param max              The maximum extends of the bounding volume
    inline void SetMax(exrPoint3 max) { m_Max = max; }

    //! @brief Test the bounding volume for intersections with a ray
    //! 
    //! This function allows us to do intersection tests with a segment of a ray in the domain
    //! of tMin and tMax, using the slab method
    //! 
    //! @param ray              The ray to test against
    //! 
    //! @return                 True if the there is an intersection
    exrBool Intersect(const Ray& ray) const;

public:
    //! @brief Combines two bounding volumes
    //!
    //! Computes a bounding volume that tightly encapsulates both input bounding volumes
    //!
    //! @return                 The combined bounding volume
    static AABB Union(const AABB& bv1, const AABB& bv2);

    //! @brief Combines two bounding volumes
    //!
    //! Computes a bounding volume that tightly encapsulates all input primitives
    //!
    //! @return                 The result bounding volume
    static AABB BoundPrimitives(const std::vector<Primitive*>& primitives);

private:
    exrPoint3 m_Min;
    exrPoint3 m_Max;
};

exrEND_NAMESPACE