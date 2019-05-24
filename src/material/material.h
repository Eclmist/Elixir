#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "core/elixir.h"

exrBEGIN_NAMESPACE

class Ray;
struct PrimitiveHitInfo;

class Material
{
public:
    virtual exrBool Scatter(const Ray& in, const PrimitiveHitInfo& hitInfo, exrVector3& attenuation, Ray& scattered) const = 0;
    virtual exrVector3 Emit() const { return exrVector3::Zero(); }
};

exrEND_NAMESPACE

#endif // !__MATERIAL_H__

