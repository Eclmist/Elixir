#include "core/system/system.h"
#include "math/math.h"
#include "boundingvolume.h"
#include "geometry/primitive.h"

exrBEGIN_NAMESPACE

exrBool BoundingVolume::Intersect(const Ray& r, exrFloat tMin, exrFloat tMax) const
{
    for (exrU32 i = 0; i < 3; i++)
    {
        exrFloat invD = 1.0f / r.m_Direction[i];
        exrFloat t0 = (m_Min[i] - r.m_Origin[i]) * invD;
        exrFloat t1 = (m_Max[i] - r.m_Origin[i]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        tMin = t0 > tMin ? t0 : tMin;
        tMax = t1 < tMax ? t1 : tMax;

        if (tMax <= tMin)
            return false;
    }

    return true;
}

BoundingVolume BoundingVolume::Combine(const BoundingVolume& bv1, const BoundingVolume& bv2)
{
    exrFloat minX, minY, minZ;
    exrFloat maxX, maxY, maxZ;

    minX = exrMin(bv1.Min().x, bv2.Min().x);
    minY = exrMin(bv1.Min().y, bv2.Min().y);
    minZ = exrMin(bv1.Min().z, bv2.Min().z);

    maxX = exrMax(bv1.Max().x, bv2.Max().x);
    maxY = exrMax(bv1.Max().y, bv2.Max().y);
    maxZ = exrMax(bv1.Max().z, bv2.Max().z);

    return BoundingVolume(exrPoint(minX, minY, minZ), exrPoint(maxX, maxY, maxZ));
}

BoundingVolume BoundingVolume::ComputeBoundingVolume(const std::vector<std::shared_ptr<Primitive>>& primitives)
{
    // We CANNOT combine with exrPoint(0) because that will make all bv extend to origin..
    if (primitives.size() <= 0)
    {
        return BoundingVolume(exrPoint::Zero(), exrPoint::Zero());
    }

    BoundingVolume combinedBv = primitives[0]->GetBoundingVolume();

    for (exrU32 i = 1; i < primitives.size(); i++)
    {
        combinedBv = Combine(combinedBv, primitives[i]->GetBoundingVolume());
    }

    return combinedBv;
}

exrEND_NAMESPACE