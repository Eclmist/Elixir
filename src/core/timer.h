#ifndef __CORE_TIMER_H__
#define __CORE_TIMER_H__

#include <iostream>
#include <string>
#include <ctime>

#define TIMER_PROFILE_CPU(a) Timer timer(a);
#define TIMER_ENDPROFILE_CPU() timer.EndTimer();

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
        clock_t timeElapsed = 1000.0 * (endTime - m_StartTime) / CLOCKS_PER_SEC;

        std::cout << m_ProcessName << " completed \t\t\t" << timeElapsed << " ms" << std::endl;
    }

private:
    std::clock_t m_StartTime;
    std::string m_ProcessName;

    bool m_HasEarlyExit = false;
};

#endif // !_CORE_TIMER_H__