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

#ifdef EXR_ENABLE_LOGGING
#include <iostream>
#define exrDebugLine(msg)                           std::cout << "[Debug] "   << msg << '\n' << std::flush;
#define exrDebug(msg)                               std::cout << "[Debug] "   << msg << std::flush;
#define exrInfoLine(msg)                            std::cout << "[Info] "    << msg << '\n' << std::flush;
#define exrInfo(msg)                                std::cout << "[Info] "    << msg << std::flush;
#define exrWarningLine(msg)                         std::cout << "[Warning] " << msg << '\n' << std::flush;
#define exrWarning(msg)                             std::cout << "[Warning] " << msg << std::flush;
#else
#define exrDebug(msg)                               ((void)0)
#define exrInfo(msg)                                ((void)0)
#endif

#ifdef EXR_ENABLE_ERRORS
#include <iostream>
#define exrError(msg)                               std::cout << "[Error] " << msg << '\n';
#else
#define exrError(msg)                               ((void)0)
#endif

#ifdef EXR_ENABLE_ASSERTS
#include <cassert>
#define exrAssert(condition, msg)                   assert(condition && msg)
#define exrSoftAssert(condition, msg)               std::cerr << "Soft assert failed: " #condition << " " << msg << '\n';
#else
#define exrSoftAssert(condition, msg)               ((void)0)
#define exrAssert(condition, msg)                   ((void)0)
#endif

// Static assert should always be there
#define exrStaticAssertMsg(condition, msg)          static_assert(condition, "static_assert: " msg)
