#ifndef __MATH_UTILS_H__
#define __MATH_UTILS_H__

#include <math.h>
#include <cstdlib>

#define NOMINMAX
#include <algorithm> // for std::min and std::max 

#define LERP(a, b, t) ((1.0f - t) * a + t * b)
#define SATURATE(x) std::max(0.0f, std::min(1.0f, x))

#define RAND01() (float)rand() / RAND_MAX

#endif // !__MATH_UTILS_H__
