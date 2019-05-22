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

#include <memory>

#include "config.h"
#include "error.h"
#include "utils.h"

#define NUM_CHANNELS 3

// CI running binary automatically
#if EXR_QUALITY_LEVEL == 0
#define OUTPUT_WIDTH 2
#define OUTPUT_HEIGHT 2
#define NUM_SAMPLES_PER_PIXEL 4
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 10
#endif

#if EXR_QUALITY_LEVEL == 5
#define OUTPUT_WIDTH 1000
#define OUTPUT_HEIGHT 600
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 1024
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 9
#endif

#if EXR_QUALITY_LEVEL == 4
#define OUTPUT_WIDTH 1000
#define OUTPUT_HEIGHT 600
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 8
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 9
#endif

#if EXR_QUALITY_LEVEL == 3
#define OUTPUT_WIDTH 500
#define OUTPUT_HEIGHT 300
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 3
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 7
#endif

#if EXR_QUALITY_LEVEL == 2
#define OUTPUT_WIDTH 250
#define OUTPUT_HEIGHT 150
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 2
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 5
#endif

#if EXR_QUALITY_LEVEL == 1
#define OUTPUT_WIDTH 500
#define OUTPUT_HEIGHT 300
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 1
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 3
#endif

#include "math/vector3.h"
#include "math/point.h"
#include <string>

exrBEGIN_NAMESPACE

typedef std::string             exrString;
typedef Vector3f                exrVector3;
typedef Point3f                 exrPoint;
typedef bool                    exrBool;
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

#include "profiling/profiler.h"

#endif // !__CORE_SYSTEM_H__
