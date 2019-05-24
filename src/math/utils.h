/* ================================================================================================ /
 ____    ____  ____   ____     ___  ____        ___   _____       _____ __ __   ____  ___ ___    ___
|    \  /    ||    \ |    \   /  _]|    \      /   \ |     |     / ___/|  |  | /    ||   |   |  /  _]
|  o  )|  o  ||  _  ||  _  | /  [_ |  D  )    |     ||   __|    (   \_ |  |  ||  o  || _   _ | /  [_
|     ||     ||  |  ||  |  ||    _]|    /     |  O  ||  |_       \__  ||  _  ||     ||  \_/  ||    _]
|  O  ||  _  ||  |  ||  |  ||   [_ |    \     |     ||   _]      /  \ ||  |  ||  _  ||   |   ||   [_
|     ||  |  ||  |  ||  |  ||     ||  .  \    |     ||  |        \    ||  |  ||  |  ||   |   ||     |
|_____||__|__||__|__||__|__||_____||__|\_|     \___/ |__|         \___||__|__||__|__||___|___||_____|

                        This file is ghetto. Remove when no longer ghetto. 
 ================================================================================================ */

// This file should probably be merged into math/math.h
#pragma once

#include "core/elixir.h"

exrBEGIN_NAMESPACE

exrFloat SchlickFresnelApproximation(exrFloat cosine, exrFloat ior)
{
    exrFloat r0 = (1.0f - ior) / (1.0f + ior);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * exrPow((1.0f - cosine), 5);
}

exrEND_NAMESPACE