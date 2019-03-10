#ifndef __MATH_UTILS_H__
#define __MATH_UTILS_H__

#include <math.h>
#include <cstdlib>

#define NOMINMAX
#include <algorithm> // for std::min and std::max 

#define LERP(a, b, t) ((1.0f - t) * a + t * b)
#define SATURATE(x) std::max(0.0f, std::min(1.0f, x))

#define RAND01() (float)rand() / RAND_MAX

Vector3f RandomInUnitSphere()
{
    Vector3f p;

    do {
        p = 2.0f * Vector3f(RAND01(), RAND01(), RAND01()) - Vector3f(1.0f);
    } while (p.SquareMagnitude() >= 1.0f);

    return p;
}

float SchlickFresnelApproximation(float cosine, float ior)
{
    float r0 = (1.0f - ior) / (1.0f + ior);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * pow((1.0f - cosine), 5);
}
#endif // !__MATH_UTILS_H__
