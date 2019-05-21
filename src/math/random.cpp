#include "core/system/system.h"
#include "random.h"
#include "math.h"

exrBEGIN_NAMESPACE

Vector3f Random::RandomInUnitSphere()
{
    Vector3f p;

    do {
        p = 2.0f * Vector3f(Random::Random01(), Random::Random01(), Random::Random01()) - Vector3f(1.0f);
    } while (p.SquareMagnitude() >= 1.0f);

    return p;
}

Vector3f Random::RandomInUnitDisc()
{
    Vector3f p;

    do {
        p = 2.0f * Vector3f(Random::Random01(), Random::Random01(), 0.0f) - Vector3f(1.0f, 1.0f, 0.0f);
    } while (Dot(p, p) >= 1.0f);

    return p;
}

exrEND_NAMESPACE