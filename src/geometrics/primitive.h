#ifndef __GEOMETRICS_GEOMETRY_H__
#define __GEOMETRICS_GEOMETRY_H__

#include <memory>
#include "math/ray.h"
#include "math/point.h"
#include "math/boundingvolume.h"

class Material;

//! A struct that contains the surface data of an intersection point
struct PrimitiveHitInfo
{
    //! The ray's t value at the point of intersection
    float t;

    //! The point of intersection
    Point3f point;
    
    //! The normal of the surface at the point of intersection
    Vector3f normal;

    //! A pointer to the material of the surface at the point of intersection
    std::shared_ptr<Material> material;
};

//! @brief A base class that all primitives should inherit from
//! 
//! A base class that all primitive shapes such as triangles and spheres should inherit from.
//! Contains various pure virtual functions that should be overridden by individual primitives,
//! such as for computing its bounding volume and handling ray intersections
class Primitive
{
public:
    //! @brief Constructs a primitive
    //! @param material         The material of the primitive
    Primitive(std::shared_ptr<Material> material) : m_Material(material) {};

public:
    //! @brief Test the geometry for intersections with a ray
    //! 
    //! This function allows us to do intersection tests with a segment of a ray in the domain
    //! of tMin and tMax
    //! 
    //! @param ray              The ray to test against
    //! @param tMin             Min t value of ray to test
    //! @param tMax             Max t value of ray to test
    //! @param hitInfo          Output struct that contains the hit information
    //! 
    //! @return                 True if the there is an intersection
    virtual bool Hit(const Ray& ray, float tMin, float tMax, PrimitiveHitInfo& hitInfo) const = 0;

    //! @brief Computes a bounding volume
    //! 
    //! Computes the a bounding volume that encapsulates the current geometry.
    //! 
    //! @param t0               Undocumented property
    //! @param t1               Undocumented property
    //! @param bv               The output bounding volume
    //!
    //! @return                 True if the bounding value can be computed with this geometry
    virtual bool ComputeBoundingVolume(float t0, float t1, BoundingVolume& bv) const = 0;

public:
    //! A pointer to the material of the sphere
    const std::shared_ptr<Material> m_Material;
};

#endif // !__GEOMETRICS_GEOMETRY_H__

