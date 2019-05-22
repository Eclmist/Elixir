#ifndef __CORE_SYSTEM_PROFILING_PROFILER_H__
#define __CORE_SYSTEM_PROFILING_PROFILER_H__

#include "timer.h"

// @brief Simple system clock based profiler
// This class will have to be redesigned when we eventually link with an
// external profiler

#define exrProfile(a)           Timer timer(a);
#define exrEndProfile()         timer.EndTimer();

#endif // !__CORE_SYSTEM_PROFILING_PROFILER_H__
