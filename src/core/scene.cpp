#include "scene.h"

void Scene::AddPrimitive(std::shared_ptr<Primitive> geometry)
{
    m_Primitives.push_back(geometry);
    m_IsDirty = true;
}

bool Scene::RaytraceScene(const Ray& viewRay, float tMin, float tMax, PrimitiveHitInfo& hitInfo)
{
    if (m_Bvh == nullptr || m_IsDirty)
    {
        m_Bvh = std::make_unique<Bvh>(m_Primitives);
        m_IsDirty = false;
    }

    return m_Bvh->Hit(viewRay, tMin, tMax, hitInfo);
}
