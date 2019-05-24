#ifndef __MATH_BOUNDING_VOLUME_H__
#define __MATH_BOUNDING_VOLUME_H__

#include "core/elixir.h"

exrBEGIN_NAMESPACE

class Primitive;

//! @brief A bounding volume that can be used in accelerators
//!
//! An axis aligned bounding volume that can be used to accelerate intersection test
//! with more complex geometry.
class BoundingVolume
{
public:
    //! @brief Constructor from two points
    //! @param min              The minimum extents of the bounding volume in world space
    //! @param max              The maximum extents of the bounding volume in world space
    BoundingVolume(const exrPoint& min = exrPoint(-1.0f), const exrPoint& max = exrPoint(1.0f))
        : m_Min(min), m_Max(max) {};

    //! @brief Copy Constructor
    //! @param copy             The bounding volume to copy from
    BoundingVolume(const BoundingVolume& copy)
        : m_Min(copy.m_Min), m_Max(copy.m_Max) {};

    //! @brief Returns minimum extents of the bounding volume in world space
    //! @return                 The minimum extends of the bounding volume
    inline exrPoint Min() const { return m_Min; }

    //! @brief Returns maximum extents of the bounding volume in world space
    //! @return                 The maximum extends of the bounding volume
    inline exrPoint Max() const { return m_Max; }

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
    inline void SetMin(exrPoint min) { m_Min = min; }

    //! @brief Sets the max extents of the bounding volume
    //! @param max              The maximum extends of the bounding volume
    inline void SetMax(exrPoint max) { m_Max = max; }

    //! @brief Test the bounding volume for intersections with a ray
    //! 
    //! This function allows us to do intersection tests with a segment of a ray in the domain
    //! of tMin and tMax, using the slab method
    //! 
    //! @param ray              The ray to test against
    //! @param tMin             Min t value of ray to test
    //! @param tMax             Max t value of ray to test
    //! 
    //! @return                 True if the there is an intersection
    exrBool Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax) const;

public:
    //! @brief Combines two bounding volumes
    //!
    //! Computes a bounding volume that tightly encapsulates both input bounding volumes
    //!
    //! @return                 The combined bounding volume
    static BoundingVolume Combine(const BoundingVolume& bv1, const BoundingVolume& bv2);

    //! @brief Combines two bounding volumes
    //!
    //! Computes a bounding volume that tightly encapsulates all input primitives
    //!
    //! @return                 The result bounding volume
    static BoundingVolume ComputeBoundingVolume(const std::vector<Primitive*>& primitives);

private:
    exrPoint m_Min;
    exrPoint m_Max;
};

exrEND_NAMESPACE

#endif // !__MATH_BOUNDING_VOLUME_H__
