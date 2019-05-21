#ifndef __CORE_SCENE_H__
#define __CORE_SCENE_H__

#include <vector>
#include "core/system/system.h"
#include "geometry/primitive.h"
#include "math/ray.h"
#include "accelerator/bvh.h"

exrBEGIN_NAMESPACE

//! @brief A scene object that owns primitives
//! 
//! A scene class that contains a collection of primitives and various helper functions
//! to interact with the collection.
class Scene
{
public:
    Scene() {}

    //! @brief Copy constructor
    //! @param copy             The object to copy
    Scene(const Scene& copy) : m_Primitives(copy.m_Primitives) {};

    Scene operator=(const Scene& copy) { return Scene(copy); }

    //! @brief Adds a primitive to the scene
    //! 
    //! This function adds a new primitive to the scene's primitive collection
    //! 
    //! @param primitive         A pointer to the primitive
    void AddPrimitive(std::shared_ptr<Primitive> primitive);

    //! @brief Raytrace through the scene and returns the info of the nearest hit point
    //! 
    //! This function and executes an intersection test with the scene objects
    //! with the input ray.
    //! 
    //! @param ray              The ray to test against
    //! @param tMin             Min t value of ray to test
    //! @param tMax             Max t value of ray to test
    //! @param hitInfo          Output struct that contains the hit information of the nearest hit point
    //! 
    //! @return                 True if the there is at least one intersection
    bool RaytraceScene(const Ray& ray, float tMin, float tMax, PrimitiveHitInfo& hitInfo) const;

    //! @brief Initializes the scene BVH if it has yet to be initialized or needs to be updated
    void InitializeBvh();

public:
    //! @brief Returns the number of primitives in the scene
    //! @return                 The number of primitives in the scene
    inline size_t GetSceneSize() const { return m_Primitives.size(); }

private:
    //! A collection of pointers that points to primitives in the scene
    std::vector<std::shared_ptr<Primitive>> m_Primitives;

    //! The accelerator to use
    std::unique_ptr<Accelerator> m_Accelerator;

    //! A flag that determines if the scene has changed since the last render
    bool m_IsDirty;
};

exrEND_NAMESPACE

#endif // !__CORE_SCENE_H__
