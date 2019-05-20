#ifndef __MATERIAL_DIFFUSE_LIGHT_H__
#define __MATERIAL_DIFFUSE_LIGHT_H__

#include "material/material.h"

class DiffuseLight : public Material
{
public:
    DiffuseLight(const Vector3f& e) : m_Emissive(e) {};

    virtual bool Scatter(const Ray& in, const PrimitiveHitInfo& hitInfo, Vector3f& attenuation, Ray& scattered) const
    {
        // Assume light source does not scatter incoming ray
        return false;
    }

    virtual Vector3f Emit() const
    {
        return m_Emissive;
    }

private:
    Vector3f m_Emissive;
};

#endif //! __MATERIAL_DIFFUSE_LIGHT_H__
