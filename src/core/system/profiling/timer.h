#ifndef __CORE_SYSTEM_PROFILING_TIMER_H__
#define __CORE_SYSTEM_PROFILING_TIMER_H__

#include <iostream>
#include <string>
#include <ctime>
#include <chrono>

exrBEGIN_NAMESPACE

class Timer
{
public:
    Timer(std::string processName = "Unnamed scope") : m_ProcessName(processName), m_StartTime(std::clock())
    {
        std::cout << "\n" << processName << " " << std::endl;
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

        std::cout << m_ProcessName << " completed \t\t\t" << timeElapsed << " ms" << std::endl;
    }

private:
    std::clock_t m_StartTime;
    std::string m_ProcessName;

    bool m_HasEarlyExit = false;
};

exrEND_NAMESPACE

#endif // !__CORE_SYSTEM_PROFILING_TIMER_H__