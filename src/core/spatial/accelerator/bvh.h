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

#include "accelerator.h"
#include "core/spatial/utils/aabb.h"

exrBEGIN_NAMESPACE

class Material;
class Primitive;

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
        //! The list of primitives that this node contains
        std::vector<Primitive*> m_Primitives;

        //! A bounding volume that contains all the objects below this node
        AABB m_BoundingVolume;

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

public:
    std::vector<Primitive*> Intersect(const Ray& ray) const override;

private:
    //! @brief A recursive function to recursively traverse nodes and check for intersection.
    //!
    //! @param node             The node to traverse down from.
    //! @param ray              The ray to test against.
    //!
    //! @return                 A collection of primitives that may pass intersection test
    //!                         against the input ray.
    static std::vector<Primitive*> TraverseNode(const BVHNode& node, const Ray& ray);

    //! @brief Recursively splits objects into equal subtrees
    //! 
    //! Split the objects into two equal subtrees on a random axis, such that
    //! the left subtree and the right subtree has the same number of elements  
    //!
    //! @param currentRoot      The current root node, whose left and right subtree we will populate
    //! @param depth            The depth of the BVH tree, used to stop recursion when m_MaxNodeDepth is reached 
    static void EqualCountSplit(BVHNode& currentRoot, exrU16 depth);
    
    //! @brief Recursively splits objects based on surface area heuristics
    //! 
    //! Computes the split based on Surface Area Heuristics
    //!
    //! @param currentRoot      The current root node, whose left and right subtree we will populate
    static void SAHSplit(BVHNode& currentRoot, exrU16 depth);

private:
    //! The root node of the BVH
    std::unique_ptr<BVHNode> m_RootNode = nullptr;
};

exrEND_NAMESPACE