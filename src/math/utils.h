#ifndef __MATH_UTILS_H__
#define __MATH_UTILS_H__

#include <math.h>
#include <cstdlib>

#define NOMINMAX
#include <algorithm> // for std::min and std::max 

#ifndef _MATH_DEFINES_DEFINED
    #define _MATH_DEFINES_DEFINED
    #define M_E        2.71828182845904523536   // e
    #define M_LOG2E    1.44269504088896340736   // log2(e)
    #define M_LOG10E   0.434294481903251827651  // log10(e)
    #define M_LN2      0.693147180559945309417  // ln(2)
    #define M_LN10     2.30258509299404568402   // ln(10)
    #define M_PI       3.14159265358979323846   // pi
    #define M_PI_2     1.57079632679489661923   // pi/2
    #define M_PI_4     0.785398163397448309616  // pi/4
    #define M_1_PI     0.318309886183790671538  // 1/pi
    #define M_2_PI     0.636619772367581343076  // 2/pi
    #define M_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
    #define M_SQRT2    1.41421356237309504880   // sqrt(2)
    #define M_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)
#endif

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

Vector3f RandomInUnitDisc()
{
    Vector3f p;

    do {
        p = 2.0f * Vector3f(RAND01(), RAND01(), 0.0f) - Vector3f(1.0f, 1.0f, 0.0f);
    } while (Dot(p, p) >= 1.0f);

    return p;
}

float SchlickFresnelApproximation(float cosine, float ior)
{
    float r0 = (1.0f - ior) / (1.0f + ior);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * pow((1.0f - cosine), 5);
}
#endif // !__MATH_UTILS_H__
