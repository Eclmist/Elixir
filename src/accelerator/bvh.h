#ifndef __ACCELERATOR_BVH_H__
#define __ACCELERATOR_BVH_H__

#include <vector>
#include "geometrics/primitive.h"

//! @brief Defines a bounding volume hierarchy
//!
//! A bounding volume hierarchy that recursively subdivides a list of objects into subgroups
//! that can accelerate ray tracing through a large collection of objects
class Bvh
{
public:
    //! @brief Constructs a BVH with a collection of objects
    //! @param objects          A collection of objects
    Bvh(std::vector<std::shared_ptr<Primitive>> objects);

    //! @brief Test the entire BVH for intersections with a ray
    //! 
    //! This function allows us to do intersection tests with a segment of a ray in the domain
    //! of tMin and tMax, against the bounding volume hierarchy. 
    //! 
    //! @param ray              The ray to test against
    //! @param tMin             Min t value of ray to test
    //! @param tMax             Max t value of ray to test
    //! @param hitInfo          Output struct that contains the hit information
    //! 
    //! @return                 True if the there is an intersection
    bool Hit(const Ray& ray, float tMin, float tMax, PrimitiveHitInfo& hitInfo) const;

public:
    //! A pointer to the left subtree of the BVH. Will be null if this is a leaf node.
    std::unique_ptr<Bvh> m_Left = nullptr;

    //! A pointer to the right subtree of the BVH. Will be null if this is a leaf node.
    std::unique_ptr<Bvh> m_Right = nullptr;

    //! A pointer to a bounding volume that contains all the objects below this node
    std::shared_ptr<BoundingVolume> m_BoundingVolume;

    //! A pointer to the primitive. Will be null unless this is a leaf node.
    std::shared_ptr<Primitive> m_Primitive;
};

#endif // !__ACCELERATOR_BVH_H__
