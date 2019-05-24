#pragma once

#include "primitive.h"

exrBEGIN_NAMESPACE

//! @brief A class that defines a sphere primitive.
//! 
//! A class that defines a sphere primitive and handles ray-sphere interactions
class Sphere: public Primitive
{
public:
    //! @brief Constructs a sphere with a center, radius and material
    //! @param center           The origin of the sphere in world space
    //! @param radius           The radius of the sphere
    //! @param material         The material of the sphere
    Sphere(const exrPoint& center, exrFloat radius, std::unique_ptr<Material> material)
        : Primitive(material), m_Center(center), m_Radius(radius)
    {
        ComputeBoundingVolume();
    };

    //! @brief Test the sphere for intersections with a ray
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
    virtual exrBool Intersect(const Ray& ray, exrFloat tMin, exrFloat tMax, PrimitiveHitInfo& hitInfo) const override;

    //! @brief Computes a bounding volume
    //! 
    //! Computes the a bounding volume that encapsulates the current sphere.
    //! 
    //! @return                 Always return true since bounding volumes can be created for spheres
    virtual exrBool ComputeBoundingVolume() override;

public:
    //! The world space position of the center of the sphere
    exrPoint m_Center;

    //! The radius of the sphere
    exrFloat m_Radius;
};

exrEND_NAMESPACE