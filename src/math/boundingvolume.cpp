#include "boundingvolume.h"
#include "geometrics\primitive.h"

bool BoundingVolume::Hit(const Ray& r, float tMin, float tMax) const
{
    for (int i = 0; i < 3; i++)
    {
        float invD = 1.0f / r.m_Direction[i];
        float t0 = (m_Min[i] - r.m_Origin[i]) * invD;
        float t1 = (m_Max[i] - r.m_Origin[i]) * invD;
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
    float minX, minY, minZ;
    float maxX, maxY, maxZ;

    minX = fmin(bv1.Min().x, bv2.Min().x);
    minY = fmin(bv1.Min().y, bv2.Min().y);
    minZ = fmin(bv1.Min().z, bv2.Min().z);

    maxX = fmax(bv1.Max().x, bv2.Max().x);
    maxY = fmax(bv1.Max().y, bv2.Max().y);
    maxZ = fmax(bv1.Max().z, bv2.Max().z);

    return BoundingVolume(Point3f(minX, minY, minZ), Point3f(maxX, maxY, maxZ));
}

BoundingVolume BoundingVolume::ComputeBoundingVolume(const std::vector<std::shared_ptr<Primitive>>& primitives)
{
    BoundingVolume combinedBv(Point3f::Zero(), Point3f::Zero());

    for (int i = 0; i < primitives.size(); i++)
    {
        combinedBv = Combine(combinedBv, *primitives[i]->GetBoundingVolume());
    }

    return combinedBv;
}
