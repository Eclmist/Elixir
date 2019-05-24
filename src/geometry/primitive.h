#pragma once

#include "core/elixir.h"
#include "material/material.h"
#include "math/boundingvolume.h"

exrBEGIN_NAMESPACE

//! A struct that contains the surface data of an intersection point
struct PrimitiveHitInfo
{
    //! The ray's t value at the point of intersection
    float m_T;

    //! The point of intersection
    exrPoint m_Point;
    
    //! The normal of the surface at the point of intersection
    exrVector3 m_Normal;

    //! A pointer to the material of the surface at the point of intersection
    Material* m_Material;
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
    Primitive(std::unique_ptr<Material>& material)
        : m_Material(std::move(material)) {};

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
    virtual bool Intersect(const Ray& ray, float tMin, float tMax, PrimitiveHitInfo& hitInfo) const = 0;

public:
    //! @brief Returns the bounding volume of the primitive
    //! @return                 A bounding volume of the primitive
    inline BoundingVolume GetBoundingVolume()
    {
        return m_BoundingVolume;
    };

protected :
    //! @brief Computes a bounding volume
    //! 
    //! Computes the a bounding volume that encapsulates the current geometry.
    //!
    //! @return                 True if the bounding volume was successfully created
    virtual bool ComputeBoundingVolume() = 0;

protected:
    //! A pointer to the material of the primitive
    const std::unique_ptr<Material> m_Material;
    
    //! A bounding volume that contains the primitive;
    BoundingVolume m_BoundingVolume;
};

exrEND_NAMESPACE