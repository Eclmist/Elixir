#ifndef __ACCELERATORS_BVH_H__
#define __ACCELERATOR_BVH_H__

#include <vector>
#include "geometry/primitive.h"

//! @brief Defines a bounding volume hierarchy
//!
//! A bounding volume hierarchy that recursively subdivides a list of objects into subgroups
//! that can accelerate ray tracing through a large collection of objects
class BVHAccelerator
{
public:
    //! Split Types
    //! SAH seems to be the most effective while EqualCount is the simplest to implement
    enum class SplitMethod { SAH, EqualCounts /*, HLBVH, Middle*/ };

    //! @brief Constructs a BVH with a collection of objects
    //! @param objects          A collection of objects
    //! @param splitMethod      Splitting algorithm to use when building the BVH
    BVHAccelerator(const std::vector<std::shared_ptr<Primitive>>& objects, const SplitMethod splitMethod = SplitMethod::SAH);

    //! @brief Test the entire BVH for intersections with a ray
    //! 
    //! Do intersection tests with a segment of a ray in the domain
    //! of tMin and tMax, against the bounding volume hierarchy. 
    //! 
    //! @param ray              The ray to test against
    //! @param tMin             Min t value of ray to test
    //! @param tMax             Max t value of ray to test
    //! @param hitInfo          Output struct that contains the hit information
    //! 
    //! @return                 True if the there are any intersections
    bool Intersect(const Ray& ray, float tMin, float tMax, PrimitiveHitInfo& hitInfo) const;

private:
    //! @brief Splits objects into two equal subtrees
    //! 
    //! Split the objects into two equal subtrees on a random axis, such that
    //! the left subtree and the right subtree has the same number of elements  
    //!
    //! @param objects          The collection of objects
    //! @param leftBucket       The left subtree to populate
    //! @param rightBucket      The right subtree to populate
    static void EqualCountSplit(
        const std::vector<std::shared_ptr<Primitive>>& objects,
        std::unique_ptr<BVHAccelerator>& leftBucket,
        std::unique_ptr<BVHAccelerator>& rightBucket);
    
    //! @brief Splits objects based on surface area heuristics
    //! 
    //! Computes the split based on Surface Area Heuristics
    //!
    //! @param objects          The collection of objects
    //! @param boundingVolume   The bounding volume of the current level node
    //! @param leftBucket       The left subtree to populate
    //! @param rightBucket      The right subtree to populate
    static void SAHSplit(
        const std::vector<std::shared_ptr<Primitive>>& objects,
        const std::shared_ptr<BoundingVolume>& boundingVolume,
        std::unique_ptr<BVHAccelerator>& leftBucket,
        std::unique_ptr<BVHAccelerator>& rightBucket);

private:
    //! A pointer to the left subtree of the BVH. Will be null if this is a leaf node.
    std::unique_ptr<BVHAccelerator> m_Left = nullptr;

    //! A pointer to the right subtree of the BVH. Will be null if this is a leaf node.
    std::unique_ptr<BVHAccelerator> m_Right = nullptr;

    //! A pointer to a bounding volume that contains all the objects below this node
    std::shared_ptr<BoundingVolume> m_BoundingVolume;

    //! A pointer to the primitive. Will be null unless this is a leaf node.
    std::shared_ptr<Primitive> m_Primitive;
};

#endif // !__ACCELERATOR_BVH_H__
