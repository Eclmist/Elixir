#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <memory>

#include "core/system/system.h"
#include "math/vector3.h"
#include "geometry/primitive.h"
#include "math/ray.h"
#include "math/random.h"

class Ray;
struct PrimitiveHitInfo;

exrBEGIN_NAMESPACE

class Material
{
public:
    virtual exrBool Scatter(const Ray& in, const PrimitiveHitInfo& hitInfo, exrVector3& attenuation, Ray& scattered) const = 0;
    virtual exrVector3 Emit() const { return exrVector3::Zero(); }
};

exrEND_NAMESPACE

#endif // !__MATERIAL_H__

