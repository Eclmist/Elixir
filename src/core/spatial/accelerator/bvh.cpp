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

#include "bvh.h"
#include "core/primitive/primitive.h"

exrBEGIN_NAMESPACE

//! Maximum primitives in a leaf node (only used by ECS)
static constexpr exrU16 MaxPrimitivesPerNode = 8;
//! Maximum depth of BVH tree
static constexpr exrU16 MaxNodeDepth = 64;

BVHAccelerator::BVHAccelerator(const std::vector<Primitive*>& objects, const SplitMethod splitMethod)
{
    exrProfile("Building BVH Accelerator");

    const auto numObjects = objects.size();

    // Create root node
    m_RootNode = std::make_unique<BVHNode>();
    m_RootNode->m_Primitives = objects;

    switch (splitMethod)
    {
    case BVHAccelerator::SplitMethod::SAH:
        m_RootNode->m_BoundingVolume = AABB::BoundPrimitives(objects);
        SAHSplit(*m_RootNode, MaxNodeDepth);
        break;
    case BVHAccelerator::SplitMethod::EqualCounts:
        EqualCountSplit(*m_RootNode, MaxNodeDepth);
        break;
    default:
        throw "Selected split method is not implemented!";
        break;
    }

    exrEndProfile();
}

std::vector<Primitive*> BVHAccelerator::Intersect(const Ray& ray) const
{
    return TraverseNode(*m_RootNode, ray);
}

std::vector<Primitive*> BVHAccelerator::TraverseNode(const BVHNode& node, const Ray& ray)
{
    // If intersect bounding volume
    if (node.m_BoundingVolume.Intersect(ray))
    {
        // reached the end of tree, return hit with primitive
        if (node.m_LeftSubtree == nullptr || node.m_RightSubtree == nullptr)
        {
            return node.m_Primitives;
        }

        // Check left and right node
        std::vector<Primitive*> leftPrims = TraverseNode(*node.m_LeftSubtree, ray);
        std::vector<Primitive*> rightPrims = TraverseNode(*node.m_RightSubtree, ray);

        if (leftPrims.size() == 0)
            return rightPrims;
        if (rightPrims.size() == 0)
            return leftPrims;
        if (leftPrims.size() == 0 && rightPrims.size() == 0)
        {
            exrWarningLine("DO NOT REMOVE THIS BRANCH! If you see this error, remove the error.");
            return leftPrims; //both is empty so just return either one
        }
        return node.m_Primitives; // both is not empty so return parent collection
    }

    // Did not intersect with parent, so return an empty collection
    return std::vector<Primitive*>();
}

void BVHAccelerator::EqualCountSplit(BVHNode& currentRoot, exrU16 depth)
{
    currentRoot.m_BoundingVolume = AABB::BoundPrimitives(currentRoot.m_Primitives);

    if (depth <= 0 || currentRoot.m_Primitives.size() <= MaxPrimitivesPerNode)
    {
        return;
    }

    const exrU64 numObjects = static_cast<exrU64>(currentRoot.m_Primitives.size());
    std::vector<Primitive*> temp = currentRoot.m_Primitives;

    // Get a random axis to split objects
    switch (exrU32(Random::UniformUInt32(2)))
    {
    case 0:
        // Split along x axis
        sort(temp.begin(), temp.end(), [](Primitive* left, Primitive* right) {
            return left->GetBoundingVolume().Min().x < right->GetBoundingVolume().Min().x;
            });
        break;
    case 1:
        // Split along y axis
        sort(temp.begin(), temp.end(), [](Primitive* left, Primitive* right) {
            return left->GetBoundingVolume().Min().y < right->GetBoundingVolume().Min().y;
            });
        break;
    default:
        // Split along z axis
        exrAssert(false, "Remove this assert!"); // TODO: make sure this is removed
        sort(temp.begin(), temp.end(), [](Primitive* left, Primitive* right) {
            return left->GetBoundingVolume().Min().z < right->GetBoundingVolume().Min().z;
            });
        break;
    }

    exrS64 halfSize = numObjects / 2;
    currentRoot.m_LeftSubtree = std::make_unique<BVHNode>();
    currentRoot.m_LeftSubtree->m_Primitives = std::vector<Primitive*>(temp.begin(), temp.begin() + halfSize);

    currentRoot.m_RightSubtree = std::make_unique<BVHNode>();
    currentRoot.m_RightSubtree->m_Primitives = std::vector<Primitive*>(temp.begin() + halfSize, temp.end());

    EqualCountSplit(*currentRoot.m_LeftSubtree, depth - 1);
    EqualCountSplit(*currentRoot.m_RightSubtree, depth - 1);
}

void BVHAccelerator::SAHSplit(BVHNode& currentRoot, exrU16 depth)
{
    const auto numObjects = currentRoot.m_Primitives.size();

    // Nothing to split
    if (numObjects <= 1 || depth <= 0)
        return;

    const exrU32 splitsPerAxis = 32;
    exrFloat bestHeuristics = MaxFloat;
    exrBool hasSplit = false;

    // for each axis
    for (exrU32 axis = 0; axis < 3; ++axis)
    {
        for (exrU32 i = 0; i < splitsPerAxis; ++i)
        {
            exrFloat splitValue = exrFloat(i) / exrFloat(splitsPerAxis);

            std::vector<Primitive*> leftObjects, rightObjects;

            for (exrU32 n = 0; n < numObjects; ++n)
            {
                if ((currentRoot.m_Primitives[n]->GetBoundingVolume().Min()[axis] - 
                    currentRoot.m_BoundingVolume.Min()[axis]) / currentRoot.m_BoundingVolume.GetExtents()[axis] < splitValue)
                {
                    leftObjects.push_back(currentRoot.m_Primitives[n]);
                }
                else
                {
                    rightObjects.push_back(currentRoot.m_Primitives[n]);
                }
            }

            // if all objects already on left side, no need to continue testing bigger split values
            if (leftObjects.size() == numObjects)
            {
                break;
            }

            AABB leftBv = AABB::BoundPrimitives(leftObjects);
            AABB rightBv = AABB::BoundPrimitives(rightObjects);

            // Get score
            exrFloat sc = currentRoot.m_BoundingVolume.GetSurfaceArea();
            exrFloat aa = leftBv.GetSurfaceArea();
            exrFloat ab = rightBv.GetSurfaceArea();
            exrFloat pa = aa / sc;         // Probably of hitting A if hit parent
            exrFloat pb = ab / sc;        // Probably of hitting B if hit parent

            auto ia = leftObjects.size();                       // Cost of intersection test on A (we assume all primitives are the same cost, like PBRT)
            auto ib = rightObjects.size();                      // Cost of intersection test on B (we assume all primitives are the same cost, like PBRT)

            exrFloat heuristics = 1 /*Ttrav*/ + pa * ia + pb * ib;

            // Determine if doing a split is worth it
            // A split is not worth it if it doesn't yield a lower cost than the parent
            if (1 + aa * ia + ab * ib >= sc * numObjects)
                continue;

            if (heuristics < bestHeuristics)
            {
                hasSplit = true;

                if (currentRoot.m_LeftSubtree == nullptr)
                    currentRoot.m_LeftSubtree = std::make_unique<BVHNode>();
                if (currentRoot.m_RightSubtree == nullptr)
                    currentRoot.m_RightSubtree = std::make_unique<BVHNode>();

                bestHeuristics = heuristics;
                currentRoot.m_LeftSubtree->m_Primitives = leftObjects;
                currentRoot.m_LeftSubtree->m_BoundingVolume = leftBv;
                currentRoot.m_RightSubtree->m_Primitives = rightObjects;
                currentRoot.m_RightSubtree->m_BoundingVolume = rightBv;
            }
        }
    }

    if (!hasSplit)
        return;

    SAHSplit(*currentRoot.m_LeftSubtree, depth - 1);
    SAHSplit(*currentRoot.m_RightSubtree, depth - 1);
}

exrEND_NAMESPACE