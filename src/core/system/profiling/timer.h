#ifndef __CORE_SYSTEM_PROFILING_TIMER_H__
#define __CORE_SYSTEM_PROFILING_TIMER_H__

#include <iostream>
#include <string>
#include <ctime>
#include <chrono>

#include "core/system/system.h"

exrBEGIN_NAMESPACE

class Timer
{
public:
    Timer(exrString processName = "Unnamed scope") : m_ProcessName(processName), m_StartTime(std::clock())
    {
        exrInfoLine("\n" << processName);
    }

    ~Timer()
    {
        if (m_HasEarlyExit)
            return;

        EndTimer();
    }

    void EndTimer()
    {
        m_HasEarlyExit = true;

        std::clock_t endTime = std::clock();
        clock_t timeElapsed = clock_t(1000) * (endTime - m_StartTime) / CLOCKS_PER_SEC;

        exrString hh, mm, ss;
        FormatTime(timeElapsed, hh, mm, ss);

        exrInfoLine(m_ProcessName << " completed \t\t\t" << "Total elapsed time: " << hh << ":" << mm << ":" << ss);
    }

    // Copy pasted from main for now
    void FormatTime(exrU64 time, exrString& hh, exrString& mm, exrString& ss) const
    {
        //3600000 milliseconds in an hour
        exrU64 hr = time / 3600000;
        time = time - 3600000 * hr;
        //60000 milliseconds in a minute
        exrU64 min = time / 60000;
        time = time - 60000 * min;

        //1000 milliseconds in a second
        exrU64 sec = time / 1000;
        time = time - 1000 * sec;

        hh = exrString(hr < 10 ? 1 : 0, '0').append(std::to_string(hr));
        mm = exrString(min < 10 ? 1 : 0, '0').append(std::to_string(min));
        ss = exrString(sec < 10 ? 1 : 0, '0').append(std::to_string(sec));
    }
private:
    std::clock_t m_StartTime;
    exrString m_ProcessName;

    exrBool m_HasEarlyExit = false;
};

exrEND_NAMESPACE

#endif // !__CORE_SYSTEM_PROFILING_TIMER_H__