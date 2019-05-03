#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "geometrics/geometry.h"
#include "math/utils.h"

class Material
{
public:
    virtual bool Scatter(const Ray& in, const GeometryHitInfo& hitInfo, Vector3f& attenuation, Ray& scattered) const = 0;

};
#endif // !__MATERIAL_H__

