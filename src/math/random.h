#ifndef __MATH_RANDOM_H__
#define __MATH_RANDOM_H__

#include "math/vector3.h"

exrBEGIN_NAMESPACE

//! @brief A class that handles various pseudo random value generations
class Random
{
public:
    //! @brief Seeds the random class
    //! @param a                The value to seed the random class
    static inline void Seed(uint32_t a) { srand(a); }

    //! @brief Generates a random value between 0 and 1
    //! @return A random float between 0 and 1
    static inline float Random01() { return (float)rand() / RAND_MAX; }

    //! @brief Generates a vector between length 0 - 1 in a random direction
    //! @return A random vector inside a unit sphere
    static Vector3f RandomInUnitSphere();

    //! @brief Generates a vector between length 0 - 1 in a random direction
    //! @return A random vector inside a unit disc
    static Vector3f RandomInUnitDisc();
};

exrEND_NAMESPACE

#endif // !__MATH_RANDOM_H__
