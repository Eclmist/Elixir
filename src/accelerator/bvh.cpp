#include <algorithm>
#include <cassert>

#include "bvh.h"
#include "math/random.h"

Bvh::Bvh(std::vector<std::shared_ptr<Primitive>> objects)
{
    const size_t numObjects = objects.size();
    assert(numObjects > 0);

    if (numObjects == 1)
    {
        m_Primitive = objects[0];
        m_BoundingVolume = m_Primitive->GetBoundingVolume();
        return;
    }
    
    // Get a random axis to split objects
    switch (int(Random::Random01() * 3))
    {
    case 0:
        // Split along x axis
        sort(objects.begin(), objects.end(), [](std::shared_ptr<Primitive> left, std::shared_ptr<Primitive> right){
            return left->GetBoundingVolume()->Min().x < right->GetBoundingVolume()->Min().x;
        });
        break;
    case 1:
        // Split along y axis
        sort(objects.begin(), objects.end(), [](std::shared_ptr<Primitive> left, std::shared_ptr<Primitive> right){
            return left->GetBoundingVolume()->Min().y < right->GetBoundingVolume()->Min().y;
        });
        break;
    default:
        // Split along z axis
        sort(objects.begin(), objects.end(), [](std::shared_ptr<Primitive> left, std::shared_ptr<Primitive> right){
            return left->GetBoundingVolume()->Min().z < right->GetBoundingVolume()->Min().z;
        });
        break;
    }

    const size_t halfSize = numObjects / 2;
    m_Left = std::make_unique<Bvh>(std::vector<std::shared_ptr<Primitive>>(objects.begin(), objects.begin() + halfSize));
    m_Right = std::make_unique<Bvh>(std::vector<std::shared_ptr<Primitive>>(objects.begin() + halfSize, objects.end()));
    m_BoundingVolume = std::make_shared<BoundingVolume>(BoundingVolume::Combine(*m_Left->m_BoundingVolume, *m_Right->m_BoundingVolume));
}

bool Bvh::Hit(const Ray& ray, float tMin, float tMax, PrimitiveHitInfo& hitInfo) const
{
    // If intersect bounding volume
    if (m_BoundingVolume->Hit(ray, tMin, tMax))
    {
        // reached the end of tree, return hit with primitive
        if (m_Left == nullptr || m_Right == nullptr)
            return m_Primitive->Hit(ray, tMin, tMax, hitInfo);

        // Check left and right node
        if (m_Left->Hit(ray, tMin, tMax, hitInfo))
        {
            m_Right->Hit(ray, tMin, hitInfo.t, hitInfo);
            return true;
        }
        else
        {
            return m_Right->Hit(ray, tMin, tMax, hitInfo);
        }
    }

    return false;
}
