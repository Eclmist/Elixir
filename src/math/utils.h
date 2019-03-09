#ifndef __MATH_UTILS_H__
#define __MATH_UTILS_H__

#include <math.h>

#define NOMINMAX
#include <algorithm> // for std::min and std::max 

#define LERP(a, b, t) ((1.0f - t) * a + t * b)
#define SATURATE(x) std::max(0.0f, std::min(1.0f, x))

#endif // !__MATH_UTILS_H__
