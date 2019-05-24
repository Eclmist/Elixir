#pragma once

#include "timer.h"

// @brief Simple system clock based profiler
// This class will have to be redesigned when we eventually link with an
// external profiler

#define exrProfile(a)           Timer timer(a);
#define exrEndProfile()         timer.EndTimer();