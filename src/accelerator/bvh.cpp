#include <algorithm>

#include "bvh.h"
#include "math/math.h"

exrBEGIN_NAMESPACE

BVHAccelerator::BVHAccelerator(const std::vector<std::shared_ptr<Primitive>>& objects, const SplitMethod splitMethod)
{
    const size_t numObjects = objects.size();

    exrAssert(numObjects > 0, "Attempting to create a BVH with zero objects! This is illegal.");

    m_BoundingVolume = std::make_shared<BoundingVolume>(BoundingVolume::ComputeBoundingVolume(objects));

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

bool BVHAccelerator::Intersect(const Ray& ray, float tMin, float tMax, PrimitiveHitInfo& hitInfo) const
{
    // If intersect bounding volume
    if (m_BoundingVolume->Intersect(ray, tMin, tMax))
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
    const std::vector<std::shared_ptr<Primitive>>& objects, 
    std::unique_ptr<BVHAccelerator>& leftBucket, 
    std::unique_ptr<BVHAccelerator>& rightBucket)
{
    const size_t numObjects = objects.size();
    std::vector<std::shared_ptr<Primitive>> temp = objects;

    // Get a random axis to split objects
    switch (int(Random::Random01() * 3))
    {
    case 0:
        // Split along x axis
        sort(temp.begin(), temp.end(), [](const std::shared_ptr<Primitive> left, const std::shared_ptr<Primitive> right) {
            return left->GetBoundingVolume()->Min().x < right->GetBoundingVolume()->Min().x;
            });
        break;
    case 1:
        // Split along y axis
        sort(temp.begin(), temp.end(), [](std::shared_ptr<Primitive> left, std::shared_ptr<Primitive> right) {
            return left->GetBoundingVolume()->Min().y < right->GetBoundingVolume()->Min().y;
            });
        break;
    default:
        // Split along z axis
        sort(temp.begin(), temp.end(), [](std::shared_ptr<Primitive> left, std::shared_ptr<Primitive> right) {
            return left->GetBoundingVolume()->Min().z < right->GetBoundingVolume()->Min().z;
            });
        break;
    }

    size_t halfSize = numObjects / 2;
    leftBucket = std::make_unique<BVHAccelerator>(std::vector<std::shared_ptr<Primitive>>(temp.begin(), temp.begin() + halfSize), SplitMethod::EqualCounts);
    rightBucket = std::make_unique<BVHAccelerator>(std::vector<std::shared_ptr<Primitive>>(temp.begin() + halfSize, temp.end()), SplitMethod::EqualCounts);
}

void BVHAccelerator::SAHSplit(
    const std::vector<std::shared_ptr<Primitive>>& objects,
    const std::shared_ptr<BoundingVolume>& boundingVolume,
    std::unique_ptr<BVHAccelerator>& leftBucket, 
    std::unique_ptr<BVHAccelerator>& rightBucket)
{
    const size_t numObjects = objects.size();
    const int splitsPerAxis = 32;
    float bestHeuristics = FLT_MAX;

    std::vector<std::shared_ptr<Primitive>> bestBucketLeft, bestBucketRight;

    // for each axis
    for (int axis = 0; axis < 3; axis++)
    {
        for (int i = 0; i < splitsPerAxis; i++)
        {
            float splitValue = float(i) / float(splitsPerAxis);

            std::vector<std::shared_ptr<Primitive>> leftObjects, rightObjects;

            for (int n = 0; n < numObjects; n++)
            {
                if ((objects[n]->GetBoundingVolume()->Min()[axis] - boundingVolume->Min()[axis]) / boundingVolume->GetExtents()[axis] < splitValue)
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
            float sc = boundingVolume->GetSurfaceArea();
            float pa = leftBv.GetSurfaceArea() / sc;        // Probably of hitting A if hit parent
            float pb = rightBv.GetSurfaceArea() / sc;       // Probably of hitting B if hit parent

            size_t ia = leftObjects.size();                 // Cost of intersection test on A (we assume all primitives are the same cost, like PBRT)
            size_t ib = rightObjects.size();                // Cost of intersection test on B (we assume all primitives are the same cost, like PBRT)

            float heuristics = 1 /*Ttrav*/ + pa * ia + pb * ib;

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