#ifndef __MATERIAL_DIFFUSE_LIGHT_H__
#define __MATERIAL_DIFFUSE_LIGHT_H__

#include "material/material.h"

exrBEGIN_NAMESPACE

class DiffuseLight : public Material
{
public:
    DiffuseLight(const exrVector3& e) : m_Emissive(e) {};

    DiffuseLight(const DiffuseLight& copy) : m_Emissive(copy.m_Emissive) {};

    virtual exrBool Scatter(const Ray& in, const PrimitiveHitInfo& hitInfo, exrVector3& attenuation, Ray& scattered) const
    {
        // Assume light source does not scatter incoming ray
        return false;
    };

    virtual exrVector3 Emit() const
    {
        return m_Emissive;
    };

private:
    exrVector3 m_Emissive;
};

exrEND_NAMESPACE

#endif //! __MATERIAL_DIFFUSE_LIGHT_H__
