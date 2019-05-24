#pragma once

#include "core/elixir.h"

exrBEGIN_NAMESPACE

struct PrimitiveHitInfo;

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
};

exrEND_NAMESPACE