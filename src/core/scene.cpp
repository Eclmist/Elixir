#include "scene.h"

exrBEGIN_NAMESPACE

void Scene::AddPrimitive(std::shared_ptr<Primitive> geometry)
{
    m_Primitives.push_back(geometry);
    m_IsDirty = true;
}

bool Scene::RaytraceScene(const Ray& ray, float tMin, float tMax, PrimitiveHitInfo& hitInfo) const
{
    return m_Accelerator->Intersect(ray, tMin, tMax, hitInfo);
}

void Scene::InitializeBvh()
{
    if (m_Accelerator == nullptr || m_IsDirty)
    {
        exrProfile("Building BVH Tree")
        m_Accelerator = std::make_unique<BVHAccelerator>(m_Primitives);
        m_IsDirty = false;
        exrEndProfile()
    }
}
 
exrEND_NAMESPACE
