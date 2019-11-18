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

#include <chrono>
#include <ctime>

#include "timer.h"

exrBEGIN_NAMESPACE

Timer::Timer(exrString processName)
    : m_ProcessName(processName)
    , m_StartTime(clock())
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

    exrS64 endTime = clock();
    exrFloat timeElapsed = 1000.0f * (endTime - m_StartTime) / CLOCKS_PER_SEC;

    exrString hh, mm, ss;
    FormatTime(timeElapsed, hh, mm, ss);

    exrInfoLine(m_ProcessName << " completed \t\t\t" << "Total elapsed time: " << hh << ":" << mm << ":" << ss);
}

void Timer::FormatTime(exrFloat ftime, exrString& hh, exrString& mm, exrString& ss) 
{
    exrS64 stime = static_cast<exrS64>(ftime);
    //3600000 milliseconds in an hour
    exrS64 hr = stime / 3600000;
    stime = stime - 3600000 * hr;

    //60000 milliseconds in a minute
    exrS64 min = stime / 60000;
    stime = stime - 60000 * min;

    //1000 milliseconds in a second
    exrS64 sec = stime / 1000;
    stime = stime - 1000 * sec;

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

