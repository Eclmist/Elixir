/*
	DO NOT EDIT: This is a CMake auto generated file
	CMake configurations can be defined here, but all other changes may be lost!
*/

#define EXR_VERSION_MAJOR				0
#define EXR_VERSION_MINOR				1
#define EXR_VERSION_PATCH				32
#define EXR_PLATFORM_NAME				"Windows"

#define EXR_QUALITY_LEVEL				0

#define EXR_PLATFORM_WIN
/* #undef EXR_PLATFORM_MAC */
/* #undef EXR_PLATFORM_LINUX */

#define EXR_ENABLE_LOGGING
#define EXR_ENABLE_ASSERTS

#define EXR_USE_NAMESPACE

/* 
    Manual configurations goes below here (in config.h.in!, not config.h!), 
    if they have to. But if they are added below here, there's almost certainly a better place elsewhere
*/

#define NUM_CHANNELS 3

// CI running binary automatically
#if EXR_QUALITY_LEVEL == 0
#define OUTPUT_WIDTH 2
#define OUTPUT_HEIGHT 2
#define NUM_SAMPLES_PER_PIXEL 4
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 10
#endif

#if EXR_QUALITY_LEVEL == 1
#define OUTPUT_WIDTH 500
#define OUTPUT_HEIGHT 300
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 1
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 3
#endif

#if EXR_QUALITY_LEVEL == 2
#define OUTPUT_WIDTH 250
#define OUTPUT_HEIGHT 150
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 2
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 5
#endif

#if EXR_QUALITY_LEVEL == 3
#define OUTPUT_WIDTH 500
#define OUTPUT_HEIGHT 300
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 3
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 7
#endif

#if EXR_QUALITY_LEVEL == 4
#define OUTPUT_WIDTH 1000
#define OUTPUT_HEIGHT 600
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 8
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 9
#endif

#if EXR_QUALITY_LEVEL == 5
#define OUTPUT_WIDTH 1000
#define OUTPUT_HEIGHT 600
#define NUM_CHANNELS 3
#define NUM_SAMPLES_PER_PIXEL 1024
#define NUM_BOUNDCE_PER_RAY 4
#define SCENE_SIZE 9
#endif
