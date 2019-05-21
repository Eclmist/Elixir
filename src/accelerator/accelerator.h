#ifndef __ACCELERATOR_H__
#define __ACCELERATOR_H__

#include <vector>
#include "geometry/primitive.h"

exrBEGIN_NAMESPACE

//! @brief Defines the base class for primitive and spacial accelerators
//!
//! TODO: Add description
class Accelerator
{
public:
    //! @brief Test the accelerator for intersections with a ray
    //! 
    //! Do intersection tests with a segment of a ray in the domain
    //! of tMin and tMax, against the accelerator. 
    //! 
    //! @param ray              The ray to test against
    //! @param tMin             Min t value of ray to test
    //! @param tMax             Max t value of ray to test
    //! @param hitInfo          Output struct that contains the hit information
    //! 
    //! @return                 True if the there are any intersections
    virtual bool Intersect(const Ray& ray, float tMin, float tMax, PrimitiveHitInfo& hitInfo) const = 0;
};

exrEND_NAMESPACE

#endif // !__ACCELERATOR_H__