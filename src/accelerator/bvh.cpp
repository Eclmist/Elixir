#include <algorithm>

#include "bvh.h"
#include "geometry/primitive.h"
#include "material/material.h"

exrBEGIN_NAMESPACE

BVHAccelerator::BVHAccelerator(const std::vector<Primitive*>& objects, const SplitMethod splitMethod)
{
    const auto numObjects = objects.size();
    exrAssert(numObjects > 0, "Attempting to create a BVH with zero objects! This is illegal.");

    // Create root node
    m_RootNode = std::make_unique<BVHNode>();
    m_RootNode->Primitives = objects;

    switch (splitMethod)
    {
    case BVHAccelerator::SplitMethod::SAH:
        m_RootNode->BoundingVolume = BoundingVolume::ComputeBoundingVolume(objects);
        SAHSplit(m_RootNode, m_MaxNodeDepth);
        break;
    case BVHAccelerator::SplitMethod::EqualCounts:
        EqualCountSplit(m_RootNode, m_MaxNodeDepth);
        break;
    default:
        throw "Selected split method is not implemented!";
        break;
    }

}

exrBool BVHAccelerator::Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax, PrimitiveHitInfo& hitInfo) const
{
    return TraverseNode(*m_RootNode, ray, tMin, tMax, hitInfo);
}

exrBool BVHAccelerator::TraverseNode(const BVHNode& node, const Ray& ray, exrFloat tMin, exrFloat tMax, PrimitiveHitInfo& hitInfo)
{
    // If intersect bounding volume
    if (node.BoundingVolume.Intersect(ray, tMin, tMax))
    {
        // reached the end of tree, return hit with primitive
        if (node.LeftSubtree == nullptr || node.RightSubtree == nullptr)
        {
            exrBool hasHit = false;
            // We may have a list of primitive, test all of them for intersection and return the closest hit
            for (Primitive* primitives : node.Primitives)
            {
                if (primitives->Intersect(ray, tMin, tMax, hitInfo))
                {
                    hasHit = true;
                    tMax = hitInfo.T;
                }
            }

            return hasHit;
        }

        // Check left and right node
        if (TraverseNode(*node.LeftSubtree, ray, tMin, tMax, hitInfo))
        {
            TraverseNode(*node.RightSubtree, ray, tMin, hitInfo.T, hitInfo);
            return true;
        }
        else
        {
            return TraverseNode(*node.RightSubtree, ray, tMin, tMax, hitInfo);
        }
    }

    return false;
}

void BVHAccelerator::EqualCountSplit(std::unique_ptr<BVHNode>& currentRoot, exrU16 depth)
{
    currentRoot->BoundingVolume = BoundingVolume::ComputeBoundingVolume(currentRoot->Primitives);

    if (depth <= 0 || currentRoot->Primitives.size() <= m_MaxPrimitivePerNode)
    {
        return;
    }

    const exrU64 numObjects = static_cast<exrU64>(currentRoot->Primitives.size());
    std::vector<Primitive*> temp = currentRoot->Primitives;

    // Get a random axis to split objects
    switch (exrU32(Random::Random01() * 3))
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
        sort(temp.begin(), temp.end(), [](Primitive* left, Primitive* right) {
            return left->GetBoundingVolume().Min().z < right->GetBoundingVolume().Min().z;
            });
        break;
    }

    exrS64 halfSize = numObjects / 2;
    currentRoot->LeftSubtree = std::make_unique<BVHNode>();
    currentRoot->LeftSubtree->Primitives = std::vector<Primitive*>(temp.begin(), temp.begin() + halfSize);

    currentRoot->RightSubtree = std::make_unique<BVHNode>();
    currentRoot->RightSubtree->Primitives = std::vector<Primitive*>(temp.begin() + halfSize, temp.end());

    EqualCountSplit(currentRoot->LeftSubtree, depth - 1);
    EqualCountSplit(currentRoot->RightSubtree, depth - 1);
}

void BVHAccelerator::SAHSplit(std::unique_ptr<BVHNode>& currentRoot, exrU16 depth)
{
    const auto numObjects = currentRoot->Primitives.size();

    if (depth <= 0 || numObjects <= m_MaxPrimitivePerNode)
    {
        return;
    }

    currentRoot->LeftSubtree = std::make_unique<BVHNode>();
    currentRoot->RightSubtree = std::make_unique<BVHNode>();

    const exrU32 splitsPerAxis = 32;
    exrFloat bestHeuristics = EXR_MAX_FLOAT;

    // for each axis
    for (exrU32 axis = 0; axis < 3; axis++)
    {
        for (exrU32 i = 0; i < splitsPerAxis; i++)
        {
            exrFloat splitValue = exrFloat(i) / exrFloat(splitsPerAxis);

            std::vector<Primitive*> leftObjects, rightObjects;

            for (exrU32 n = 0; n < numObjects; n++)
            {
                if ((currentRoot->Primitives[n]->GetBoundingVolume().Min()[axis] - currentRoot->BoundingVolume.Min()[axis]) / currentRoot->BoundingVolume.GetExtents()[axis] < splitValue)
                {
                    leftObjects.push_back(currentRoot->Primitives[n]);
                }
                else
                {
                    rightObjects.push_back(currentRoot->Primitives[n]);
                }
            }

            BoundingVolume leftBv = BoundingVolume::ComputeBoundingVolume(leftObjects);
            BoundingVolume rightBv = BoundingVolume::ComputeBoundingVolume(rightObjects);

            // Get score
            exrFloat sc = currentRoot->BoundingVolume.GetSurfaceArea();
            exrFloat pa = leftBv.GetSurfaceArea() / sc;         // Probably of hitting A if hit parent
            exrFloat pb = rightBv.GetSurfaceArea() / sc;        // Probably of hitting B if hit parent

            auto ia = leftObjects.size();                       // Cost of intersection test on A (we assume all primitives are the same cost, like PBRT)
            auto ib = rightObjects.size();                      // Cost of intersection test on B (we assume all primitives are the same cost, like PBRT)

            exrFloat heuristics = 1 /*Ttrav*/ + pa * ia + pb * ib;

            if (heuristics < bestHeuristics)
            {
                bestHeuristics = heuristics;
                currentRoot->LeftSubtree->Primitives = leftObjects;
                currentRoot->LeftSubtree->BoundingVolume = leftBv;
                currentRoot->RightSubtree->Primitives = rightObjects;
                currentRoot->RightSubtree->BoundingVolume = rightBv;
            }

            // if all objects already on left side, no need to continue testing bigger split values
            if (leftObjects.size() == numObjects)
            {
                break;
            }
        }
    }

    SAHSplit(currentRoot->LeftSubtree, depth - 1);
    SAHSplit(currentRoot->RightSubtree, depth - 1);
}

exrEND_NAMESPACE