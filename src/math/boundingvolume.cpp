#include "boundingvolume.h"

bool BoundingVolume::Hit(const Ray& r, float tMin, float tMax)
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
