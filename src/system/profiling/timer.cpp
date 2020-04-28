/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019-2020 Samuel Van Allen - All rights reserved.

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

#include <chrono>
#include <ctime>

#include "timer.h"

exrBEGIN_NAMESPACE

Timer::Timer(exrString processName)
    : m_ProcessName(processName)
    , m_StartTime(std::chrono::steady_clock::now())
    , m_HasEarlyExit(false)
{
    exrInfoLine(processName);
}

Timer::~Timer()
{
    if (m_HasEarlyExit)
        return;

    EndTimer();
}

void Timer::EndTimer()
{
    m_HasEarlyExit = true;

    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    exrS64 timeElapsed = static_cast<exrS64>((endTime - m_StartTime) / std::chrono::milliseconds(1));

    exrString hh, mm, ss;
    FormatTime(timeElapsed, hh, mm, ss);

#ifdef EXR_PLATFORM_WIN
    exrInfoLine("\t   -> " << "Total elapsed time: " << hh << ":" << mm << ":" << ss);
#else
    exrInfoLine("\t   ↳ " << "Total elapsed time: " << hh << ":" << mm << ":" << ss);
#endif // EXR_PLATFORM_WIN
}

void Timer::FormatTime(exrS64 ftime, exrString& hh, exrString& mm, exrString& ss) 
{
    //3600000 milliseconds in an hour
    exrS64 hr = ftime / 3600000;
    ftime = ftime - 3600000 * hr;

    //60000 milliseconds in a minute
    exrS64 min = ftime / 60000;
    ftime = ftime - 60000 * min;

    //1000 milliseconds in a second
    exrS64 sec = ftime / 1000;
    ftime = ftime - 1000 * sec;

    hh = exrString(hr < 10 ? 1 : 0, '0').append(std::to_string(hr));
    mm = exrString(min < 10 ? 1 : 0, '0').append(std::to_string(min));
    ss = exrString(sec < 10 ? 1 : 0, '0').append(std::to_string(sec));
}

exrU64 Timer::TimeSinceEpochMillisec()
{
    using namespace std::chrono;
    exrU64 time = static_cast<exrU64>(system_clock::now().time_since_epoch() / milliseconds(1));
    return time;
}

exrEND_NAMESPACE

