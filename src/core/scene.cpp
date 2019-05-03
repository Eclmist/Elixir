#include "scene.h"

void Scene::AddPrimitive(std::unique_ptr<Geometry> geometry)
{
    m_Primitives.push_back(std::move(geometry));
}

bool Scene::RaytraceScene(const Ray& viewRay, float tMin, float tMax, GeometryHitInfo& hitInfo) const
{
    GeometryHitInfo tempHitInfo;

    bool hitAnything = false;

    float nearestDist = tMax;

    for (int i = 0; i < m_Primitives.size(); i++)
    {
        if (m_Primitives[i]->Hit(viewRay, tMin, nearestDist, tempHitInfo))
        {
            hitAnything = true;
            nearestDist = tempHitInfo.t;
            hitInfo = tempHitInfo;
        }
    }

    return hitAnything;
}
