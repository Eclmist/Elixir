#ifndef __ACCELERATOR_H__
#define __ACCELERATOR_H__

#include <vector>
#include "core/system/system.h"
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
    virtual exrBool Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax, PrimitiveHitInfo& hitInfo) const = 0;

protected:
    //! A pointer to the primitive. Will be null unless this is a leaf node.
    Primitive* m_Primitive;

    //! A bounding volume that contains all the objects below this node
    BoundingVolume m_BoundingVolume;
};

exrEND_NAMESPACE

#endif // !__ACCELERATOR_H__