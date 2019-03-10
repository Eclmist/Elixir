#ifndef __CORE_SCENE_H__
#define __CORE_SCENE_H__

#include <vector>
#include "geometrics/geometry.h"
#include "math/ray.h"

class Scene
{
public:
    Scene();
    ~Scene();
    
    void AddPrimitive(Geometry* geometry);

    bool RaytraceScene(const Ray& viewRay, GeometryHitInfo& hitInfo) const;

private:
    std::vector<Geometry*> m_Primitives;
};

#endif // !__CORE_SCENE_H__
