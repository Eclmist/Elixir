#ifndef __MATH_BOUNDING_VOLUME_H__
#define __MATH_BOUNDING_VOLUME_H__

#include "point.h"
#include "ray.h"

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
    BoundingVolume(const Point3f& min = Point3f(-1.0f), const Point3f& max = Point3f(1.0f))
        : m_Min(min), m_Max(max) {};

    //! @brief Returns minimum extents of the bounding volume in world space
    //! @return                 The minimum extends of the bounding volume
    Point3f Min() const { return m_Min; }

    //! @brief Returns maximum extents of the bounding volume in world space
    //! @return                 The maximum extends of the bounding volume
    Point3f Max() const { return m_Max; }

    //! @brief Test the bounding volume for intersections with a ray
    //! 
    //! This function allows us to do intersection tests with a segment of a ray in the domain
    //! of tMin and tMax
    //! 
    //! @param ray              The ray to test against
    //! @param tMin             Min t value of ray to test
    //! @param tMax             Max t value of ray to test
    //! 
    //! @return                 True if the there is an intersection
    bool Hit(const Ray& r, float tMin, float tMax);

private:
    Point3f m_Min;
    Point3f m_Max;
};

#endif // !__MATH_BOUNDING_VOLUME_H__
