#include "bvh.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "geometry/primitive.h"
#include "math/random.h"
#include "math/ray.h"

exrBEGIN_NAMESPACE

BVHAccelerator::BVHAccelerator(const std::vector<Primitive*>& objects, const SplitMethod splitMethod)
{
    const auto numObjects = objects.size();
    exrAssert(numObjects > 0, "Attempting to create a BVH with zero objects! This is illegal.");


    m_BoundingVolume = BoundingVolume::ComputeBoundingVolume(objects);

    if (numObjects == 1)
    {
        m_Primitive = objects[0];
        return;
    }
    
    switch (splitMethod)
    {
    case BVHAccelerator::SplitMethod::SAH:
        SAHSplit(objects, m_BoundingVolume, m_Left, m_Right);
        break;
    case BVHAccelerator::SplitMethod::EqualCounts:
        EqualCountSplit(objects, m_Left, m_Right);
        break;
    default:
        throw "Selected split method is not implemented!";
        break;
    }

}

exrBool BVHAccelerator::Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax, PrimitiveHitInfo& hitInfo) const
{
    // If intersect bounding volume
    if (m_BoundingVolume.Intersect(ray, tMin, tMax))
    {
        // reached the end of tree, return hit with primitive
        if (m_Left == nullptr || m_Right == nullptr)
            return m_Primitive->Intersect(ray, tMin, tMax, hitInfo);

        // Check left and right node
        if (m_Left->Intersect(ray, tMin, tMax, hitInfo))
        {
            m_Right->Intersect(ray, tMin, hitInfo.t, hitInfo);
            return true;
        }
        else
        {
            return m_Right->Intersect(ray, tMin, tMax, hitInfo);
        }
    }

    return false;
}

void BVHAccelerator::EqualCountSplit(
    const std::vector<Primitive*>& objects,
    std::unique_ptr<BVHAccelerator>& leftBucket,
    std::unique_ptr<BVHAccelerator>& rightBucket)
{
    const exrU64 numObjects = static_cast<exrU64>(objects.size());
    std::vector<Primitive*> temp = objects;

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
    leftBucket = std::make_unique<BVHAccelerator>(std::vector<Primitive*>(temp.begin(), temp.begin() + halfSize), SplitMethod::EqualCounts);
    rightBucket = std::make_unique<BVHAccelerator>(std::vector<Primitive*>(temp.begin() + halfSize, temp.end()), SplitMethod::EqualCounts);
}

void BVHAccelerator::SAHSplit(
    const std::vector<Primitive*>& objects,
    const BoundingVolume& boundingVolume,
    std::unique_ptr<BVHAccelerator>& leftBucket, 
    std::unique_ptr<BVHAccelerator>& rightBucket)
{
    const auto numObjects = objects.size();
    const exrU32 splitsPerAxis = 32;
    exrFloat bestHeuristics = EXR_MAX_FLOAT;

    std::vector<Primitive*> bestBucketLeft, bestBucketRight;

    // for each axis
    for (exrU32 axis = 0; axis < 3; axis++)
    {
        for (exrU32 i = 0; i < splitsPerAxis; i++)
        {
            exrFloat splitValue = exrFloat(i) / exrFloat(splitsPerAxis);

            std::vector<Primitive*> leftObjects, rightObjects;

            for (exrU32 n = 0; n < numObjects; n++)
            {
                if ((objects[n]->GetBoundingVolume().Min()[axis] - boundingVolume.Min()[axis]) / boundingVolume.GetExtents()[axis] < splitValue)
                {
                    leftObjects.push_back(objects[n]);
                }
                else
                {
                    rightObjects.push_back(objects[n]);
                }
            }

            BoundingVolume leftBv = BoundingVolume::ComputeBoundingVolume(leftObjects);
            BoundingVolume rightBv = BoundingVolume::ComputeBoundingVolume(rightObjects);

            // Get score
            exrFloat sc = boundingVolume.GetSurfaceArea();
            exrFloat pa = leftBv.GetSurfaceArea() / sc;         // Probably of hitting A if hit parent
            exrFloat pb = rightBv.GetSurfaceArea() / sc;        // Probably of hitting B if hit parent

            auto ia = leftObjects.size();                     // Cost of intersection test on A (we assume all primitives are the same cost, like PBRT)
            auto ib = rightObjects.size();                    // Cost of intersection test on B (we assume all primitives are the same cost, like PBRT)

            exrFloat heuristics = 1 /*Ttrav*/ + pa * ia + pb * ib;

            if (heuristics < bestHeuristics)
            {
                bestHeuristics = heuristics;
                bestBucketRight = rightObjects;
                bestBucketLeft = leftObjects;
            }

            // if all objects already on left side, no need to keep increasing the split value
            if (leftObjects.size() == numObjects)
            {
                break;
            }
        }
    }

    if (bestBucketLeft.size() == 0 || bestBucketRight.size() == 0)
    {
        // Fallback to equal split
        EqualCountSplit(objects, leftBucket, rightBucket);
        return;
    }

    leftBucket = std::make_unique<BVHAccelerator>(bestBucketLeft, SplitMethod::SAH);
    rightBucket = std::make_unique<BVHAccelerator>(bestBucketRight, SplitMethod::SAH);;
}

exrEND_NAMESPACE