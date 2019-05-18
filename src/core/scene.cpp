#include "scene.h"
#include "core/timer.h"

void Scene::AddPrimitive(std::shared_ptr<Primitive> geometry)
{
    m_Primitives.push_back(geometry);
    m_IsDirty = true;
}

bool Scene::RaytraceScene(const Ray& ray, float tMin, float tMax, PrimitiveHitInfo& hitInfo) const
{
    return m_Bvh->Intersect(ray, tMin, tMax, hitInfo);
}

void Scene::InitializeBvh()
{
    if (m_Bvh == nullptr || m_IsDirty)
    {
        TIMER_PROFILE_CPU("Building BVH Tree")

        m_Bvh = std::make_unique<BVHAccelerator>(m_Primitives);
        m_IsDirty = false;
    }
}
 