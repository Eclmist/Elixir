/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019 Samuel Van Allen - All rights reserved.

    Elixir is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <memory>
#include <vector>
#include <math.h>
#include <atomic>
#include <condition_variable>
#include <mutex>

#include "system/config.h"
#include "system/error.h"
#include "system/utils.h"
#include "system/types.h"
#include "system/profiling/profiler.h"

#include "math/math.h"
#include "math/vector2.h"
#include "math/vector3.h"
#include "math/point2.h"
#include "math/point3.h"
#include "math/random.h"
#include "math/matrix4x4.h"

#include "ray.h"
#include "interaction.h"
//#include "queue.h"
//#include "threadpool.h"
