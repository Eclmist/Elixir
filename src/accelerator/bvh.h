#pragma once

#include "accelerator.h"
#include "math/boundingvolume.h"

exrBEGIN_NAMESPACE

class Material;
class Primitive;
struct PrimitiveHitInfo;

//! @brief Defines a bounding volume hierarchy
//!
//! A bounding volume hierarchy that recursively subdivides a list of objects into subgroups
//! that can accelerate ray tracing through a large collection of objects
class BVHAccelerator : public Accelerator
{
public:
    //! @brief A single BVH Node
    struct BVHNode
    {
        //! The list of primitive that this node contains
        std::vector<Primitive*> m_Primitives;

        //! A bounding volume that contains all the objects below this node
        BoundingVolume m_BoundingVolume;

        //! A pointer to the left subtree of the BVH. Will be null if this is a leaf node.
        std::unique_ptr<BVHNode> m_LeftSubtree = nullptr;

        //! A pointer to the right subtree of the BVH. Will be null if this is a leaf node.
        std::unique_ptr<BVHNode> m_RightSubtree = nullptr;
    };

    //! Split Types
    //! SAH seems to be the most effective while EqualCount is the simplest to implement
    enum class SplitMethod { SAH, EqualCounts /*, HLBVH, Middle*/ };

    //! @brief Constructs a BVH with a collection of objects
    //! @param objects          A collection of objects
    //! @param splitMethod      Splitting algorithm to use when building the BVH
    BVHAccelerator(const std::vector<Primitive*>& objects, const SplitMethod splitMethod = SplitMethod::SAH);

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
    virtual exrBool Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax, PrimitiveHitInfo& hitInfo) const override;

private:
    //! @brief A recursive function to recursively traverse nodes and check for intersection
    //!
    //! @param node             The node to traverse down from
    //! @param ray              The ray to test against
    //! @param tMin             Min t value of ray to test
    //! @param tMax             Max t value of ray to test
    //! @param hitInfo          Output struct that contains the hit information
    //!
    //! @return                 True if the there are any intersections
    static exrBool TraverseNode(const BVHNode& node, const Ray& ray, exrFloat tMin, exrFloat tMax, PrimitiveHitInfo& hitInfo);

    //! @brief Recursively splits objects into equal subtrees
    //! 
    //! Split the objects into two equal subtrees on a random axis, such that
    //! the left subtree and the right subtree has the same number of elements  
    //!
    //! @param currentRoot      The current root node, whose left and right subtree we will populate
    //! @param depth            The depth of the BVH tree, used to stop recursion when m_MaxNodeDepth is reached 
    static void EqualCountSplit(std::unique_ptr<BVHNode>& currentRoot, exrU16 depth);
    
    //! @brief Recursively splits objects based on surface area heuristics
    //! 
    //! Computes the split based on Surface Area Heuristics
    //!
    //! @param currentRoot      The current root node, whose left and right subtree we will populate
    //! @param depth            The depth of the BVH tree, used to stop recursion when m_MaxNodeDepth is reached 
    static void SAHSplit(std::unique_ptr<BVHNode>& currentRoot, exrU16 depth);

private:
    //! Maximum primitives in a leaf node
    static const exrU16 m_MaxPrimitivePerNode = 8;

    //! Maximum depth of BVH tree
    static const exrU16 m_MaxNodeDepth = 16; 

private:
    //! The root node of the BVH
    std::unique_ptr<BVHNode> m_RootNode = nullptr;
};

exrEND_NAMESPACE