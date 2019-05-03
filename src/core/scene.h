#ifndef __CORE_SCENE_H__
#define __CORE_SCENE_H__

#include <vector>
#include "geometrics/geometry.h"
#include "math/ray.h"

class Scene
{
public:
    void AddPrimitive(std::unique_ptr<Geometry> geometry);
    bool RaytraceScene(const Ray& viewRay, float tMin, float tMax, GeometryHitInfo& hitInfo) const;

private:
    std::vector<std::unique_ptr<Geometry>> m_Primitives;
};

#endif // !__CORE_SCENE_H__
