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

#include <algorithm>

#include "bvh.h"
#include "shape/shape.h"
#include "material/material.h"

exrBEGIN_NAMESPACE

//! Maximum shapes in a leaf node
static constexpr exrU16 MaxShapesPerNode = 1;

//! Maximum depth of BVH tree
static constexpr exrU16 MaxNodeDepth = 16;

BVHAccelerator::BVHAccelerator(const std::vector<Shape*>& objects, const SplitMethod splitMethod)
{
    const auto numObjects = objects.size();
    exrAssert(numObjects > 0, "Attempting to create a BVH with zero objects! This is illegal.");

    // Create root node
    m_RootNode = std::make_unique<BVHNode>();
    m_RootNode->m_Shapes = objects;

    switch (splitMethod)
    {
    case BVHAccelerator::SplitMethod::SAH:
        m_RootNode->m_BoundingVolume = AABB::BoundShapes(objects);
        SAHSplit(m_RootNode, MaxNodeDepth);
        break;
    case BVHAccelerator::SplitMethod::EqualCounts:
        EqualCountSplit(m_RootNode, MaxNodeDepth);
        break;
    default:
        throw "Selected split method is not implemented!";
        break;
    }

}

exrBool BVHAccelerator::Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax, Interaction& hitInfo) const
{
    return TraverseNode(*m_RootNode, ray, tMin, tMax, hitInfo);
}

exrBool BVHAccelerator::TraverseNode(const BVHNode& node, const Ray& ray, exrFloat tMin, exrFloat tMax, Interaction& hitInfo)
{
    // If intersect bounding volume
    if (node.m_BoundingVolume.Intersect(ray, tMin, tMax))
    {
        // reached the end of tree, return hit with shape
        if (node.m_LeftSubtree == nullptr || node.m_RightSubtree == nullptr)
        {
            exrBool hasHit = false;
            // We may have a list of shape, test all of them for intersection and return the closest hit
            for (Shape* shapes : node.m_Shapes)
            {
                if (shapes->Intersect(ray, tMin, tMax, hitInfo))
                {
                    hasHit = true;
                    tMax = hitInfo.m_Time;
                }
            }

            return hasHit;
        }

        // Check left and right node
        if (TraverseNode(*node.m_LeftSubtree, ray, tMin, tMax, hitInfo))
        {
            TraverseNode(*node.m_RightSubtree, ray, tMin, hitInfo.m_Time, hitInfo);
            return true;
        }
        else
        {
            return TraverseNode(*node.m_RightSubtree, ray, tMin, tMax, hitInfo);
        }
    }

    return false;
}

void BVHAccelerator::EqualCountSplit(std::unique_ptr<BVHNode>& currentRoot, exrU16 depth)
{
    currentRoot->m_BoundingVolume = AABB::BoundShapes(currentRoot->m_Shapes);

    if (depth <= 0 || currentRoot->m_Shapes.size() <= MaxShapesPerNode)
    {
        return;
    }

    const exrU64 numObjects = static_cast<exrU64>(currentRoot->m_Shapes.size());
    std::vector<Shape*> temp = currentRoot->m_Shapes;

    // Get a random axis to split objects
    switch (exrU32(Random::Random01() * 3))
    {
    case 0:
        // Split along x axis
        sort(temp.begin(), temp.end(), [](Shape* left, Shape* right) {
            return left->GetBoundingVolume().Min().x < right->GetBoundingVolume().Min().x;
            });
        break;
    case 1:
        // Split along y axis
        sort(temp.begin(), temp.end(), [](Shape* left, Shape* right) {
            return left->GetBoundingVolume().Min().y < right->GetBoundingVolume().Min().y;
            });
        break;
    default:
        // Split along z axis
        sort(temp.begin(), temp.end(), [](Shape* left, Shape* right) {
            return left->GetBoundingVolume().Min().z < right->GetBoundingVolume().Min().z;
            });
        break;
    }

    exrS64 halfSize = numObjects / 2;
    currentRoot->m_LeftSubtree = std::make_unique<BVHNode>();
    currentRoot->m_LeftSubtree->m_Shapes = std::vector<Shape*>(temp.begin(), temp.begin() + halfSize);

    currentRoot->m_RightSubtree = std::make_unique<BVHNode>();
    currentRoot->m_RightSubtree->m_Shapes = std::vector<Shape*>(temp.begin() + halfSize, temp.end());

    EqualCountSplit(currentRoot->m_LeftSubtree, depth - 1);
    EqualCountSplit(currentRoot->m_RightSubtree, depth - 1);
}

void BVHAccelerator::SAHSplit(std::unique_ptr<BVHNode>& currentRoot, exrU16 depth)
{
    const auto numObjects = currentRoot->m_Shapes.size();

    if (depth <= 0 || numObjects <= MaxShapesPerNode)
    {
        return;
    }

    currentRoot->m_LeftSubtree = std::make_unique<BVHNode>();
    currentRoot->m_RightSubtree = std::make_unique<BVHNode>();

    const exrU32 splitsPerAxis = 32;
    exrFloat bestHeuristics = EXR_MAX_FLOAT;

    // for each axis
    for (exrU32 axis = 0; axis < 3; axis++)
    {
        for (exrU32 i = 0; i < splitsPerAxis; i++)
        {
            exrFloat splitValue = exrFloat(i) / exrFloat(splitsPerAxis);

            std::vector<Shape*> leftObjects, rightObjects;

            for (exrU32 n = 0; n < numObjects; n++)
            {
                if ((currentRoot->m_Shapes[n]->GetBoundingVolume().Min()[axis] - currentRoot->m_BoundingVolume.Min()[axis]) / currentRoot->m_BoundingVolume.GetExtents()[axis] < splitValue)
                {
                    leftObjects.push_back(currentRoot->m_Shapes[n]);
                }
                else
                {
                    rightObjects.push_back(currentRoot->m_Shapes[n]);
                }
            }

            AABB leftBv = AABB::BoundShapes(leftObjects);
            AABB rightBv = AABB::BoundShapes(rightObjects);

            // Get score
            exrFloat sc = currentRoot->m_BoundingVolume.GetSurfaceArea();
            exrFloat pa = leftBv.GetSurfaceArea() / sc;         // Probably of hitting A if hit parent
            exrFloat pb = rightBv.GetSurfaceArea() / sc;        // Probably of hitting B if hit parent

            auto ia = leftObjects.size();                       // Cost of intersection test on A (we assume all shapes are the same cost, like PBRT)
            auto ib = rightObjects.size();                      // Cost of intersection test on B (we assume all shapes are the same cost, like PBRT)

            exrFloat heuristics = 1 /*Ttrav*/ + pa * ia + pb * ib;

            if (heuristics < bestHeuristics)
            {
                bestHeuristics = heuristics;
                currentRoot->m_LeftSubtree->m_Shapes = leftObjects;
                currentRoot->m_LeftSubtree->m_BoundingVolume = leftBv;
                currentRoot->m_RightSubtree->m_Shapes = rightObjects;
                currentRoot->m_RightSubtree->m_BoundingVolume = rightBv;
            }

            // if all objects already on left side, no need to continue testing bigger split values
            if (leftObjects.size() == numObjects)
            {
                break;
            }
        }
    }

    SAHSplit(currentRoot->m_LeftSubtree, depth - 1);
    SAHSplit(currentRoot->m_RightSubtree, depth - 1);
}

exrEND_NAMESPACE