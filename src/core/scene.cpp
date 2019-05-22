#include "scene.h"

#include "accelerator/bvh.h"
#include "geometry/primitive.h"

exrBEGIN_NAMESPACE

void Scene::AddPrimitive(std::unique_ptr<Primitive> primitive) 
{
    m_Primitives.push_back(std::move(primitive));
    m_IsDirty = true;
}

bool Scene::RaytraceScene(const Ray& ray, exrFloat tMin, exrFloat tMax, PrimitiveHitInfo& hitInfo) const
{
    return m_Accelerator->Intersect(ray, tMin, tMax, hitInfo);
}

void Scene::InitializeBvh()
{
    if (m_Accelerator == nullptr || m_IsDirty)
    {
        exrProfile("Building BVH Tree")

        std::vector<Primitive*> primitivePtrs;

        // shallow copy pointer values to be used by bvh accel
        for (exrU32 i = 0; i < m_Primitives.size(); i++)
        {
            primitivePtrs.push_back(m_Primitives[i].get());
        }

        m_Accelerator = std::make_unique<BVHAccelerator>(primitivePtrs);
        m_IsDirty = false;
        exrEndProfile()
    }
}

exrEND_NAMESPACE
