#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "geometrics/Primitive.h"
#include "math/utils.h"

class Material
{
public:
    virtual bool Scatter(const Ray& in, const PrimitiveHitInfo& hitInfo, Vector3f& attenuation, Ray& scattered) const = 0;
    virtual Vector3f Emit() const { return Vector3f::Zero(); }
};
#endif // !__MATERIAL_H__

