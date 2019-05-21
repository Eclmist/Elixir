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


#ifndef __CORE_SYSTEM_H__
#define __CORE_SYSTEM_H__

#define EXR_USE_NAMESPACE

#include "error.h"
#include "utils.h"
#include "profiling/profiler.h"

#ifdef EXR_QUALITY_ULTRA
#define OUTPUT_WIDTH 1000
#define OUTPUT_HEIGHT 600
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 1024
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 9
#endif
#ifdef EXR_QUALITY_HIGH
#define OUTPUT_WIDTH 1000
#define OUTPUT_HEIGHT 600
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 8
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 9
#endif
#ifdef EXR_QUALITY_MEDIUM
#define OUTPUT_WIDTH 500
#define OUTPUT_HEIGHT 300
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 3
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 7
#endif
#ifdef EXR_QUALITY_LOW
#define OUTPUT_WIDTH 250
#define OUTPUT_HEIGHT 150
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 2
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 5
#endif
#ifdef EXR_QUALITY_PREVIEW
#define OUTPUT_WIDTH 500
#define OUTPUT_HEIGHT 300
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 1
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 3
#endif

#include "math/vector3.h"
#include <string>

exrBEGIN_NAMESPACE

typedef std::string             extString;
typedef Vector3f                exrVector3;
typedef float                   exrFloat;
typedef float                   exrF32;
typedef double                  exrF64;
typedef unsigned char           exrByte;
typedef unsigned char           exrChar;
typedef unsigned int            exrU32;
typedef int                     exrS32;
typedef unsigned long           exrU64;
typedef long                    exrS64;

exrEND_NAMESPACE

#endif // !__CORE_SYSTEM_H__
