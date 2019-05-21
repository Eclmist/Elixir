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
    static inline void Seed(exrU32 a) { srand(a); }

    //! @brief Generates a random value between 0 and 1
    //! @return A random exrFloat between 0 and 1
    static inline exrFloat Random01() { return (exrFloat)rand() / RAND_MAX; }

    //! @brief Generates a vector between length 0 - 1 in a random direction
    //! @return A random vector inside a unit sphere
    static exrVector3 RandomInUnitSphere();

    //! @brief Generates a vector between length 0 - 1 in a random direction
    //! @return A random vector inside a unit disc
    static exrVector3 RandomInUnitDisc();
};

exrEND_NAMESPACE

#endif // !__MATH_RANDOM_H__
