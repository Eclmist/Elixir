#ifndef __CORE_SCENE_H__
#define __CORE_SCENE_H__

#include <vector>
#include "geometrics/Primitive.h"
#include "math/ray.h"

//! @brief A scene object that owns primitives
//! 
//! A scene class that contains a collection of primitives and various helper functions
//! to interact with the collection.
class Scene
{
public:
    //! @brief Adds a primitive to the scene
    //! 
    //! This function adds a new primitive to the scene's primitive collection
    //! 
    //! @param primitive         A pointer to the primitive
    void AddPrimitive(std::unique_ptr<Primitive> primitive);

    //! @brief Raytrace through the scene and returns the info of the nearest hit point
    //! 
    //! This function iterates through every primitive in the scene and executes an intersection test
    //! with the ray, returning true if there is at least one intersection. The info of the closest
    //! intersection point is then stored in hitInfo.
    //! 
    //! @param ray              The ray to test against
    //! @param tMin             Min t value of ray to test
    //! @param tMax             Max t value of ray to test
    //! @param hitInfo          Output struct that contains the hit information of the nearest hit point
    //! 
    //! @return                 True if the there is at least one intersection
    bool RaytraceScene(const Ray& ray, float tMin, float tMax, PrimitiveHitInfo& hitInfo) const;

private:
    //! A collection of pointers that points to primitives in the scene
    std::vector<std::unique_ptr<Primitive>> m_Primitives;
};

#endif // !__CORE_SCENE_H__
