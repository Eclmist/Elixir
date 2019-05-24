#include "random.h"

exrBEGIN_NAMESPACE

exrVector3 Random::RandomInUnitSphere()
{
    exrVector3 p;

    do {
        p = 2.0f * exrVector3(Random::Random01(), Random::Random01(), Random::Random01()) - exrVector3(1.0f);
    } while (p.SquareMagnitude() >= 1.0f);

    return p;
}

exrVector3 Random::RandomInUnitDisc()
{
    exrVector3 p;

    do {
        p = 2.0f * exrVector3(Random::Random01(), Random::Random01(), 0.0f) - exrVector3(1.0f, 1.0f, 0.0f);
    } while (Dot(p, p) >= 1.0f);

    return p;
}

exrEND_NAMESPACE