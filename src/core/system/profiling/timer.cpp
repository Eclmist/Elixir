#include <chrono>
#include <ctime>
#include "timer.h"

exrBEGIN_NAMESPACE

Timer::Timer(exrString processName /*= "Unamed Process" */)
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
    exrS64 timeElapsed = exrS64(1000) * (endTime - m_StartTime) / CLOCKS_PER_SEC;

    exrString hh, mm, ss;
    FormatTime(timeElapsed, hh, mm, ss);

    exrInfoLine(m_ProcessName << " completed \t\t\t" << "Total elapsed time: " << hh << ":" << mm << ":" << ss);
}

void Timer::FormatTime(exrS64 time, exrString& hh, exrString& mm, exrString& ss) 
{
    //3600000 milliseconds in an hour
    exrS64 hr = time / 3600000;
    time = time - 3600000 * hr;

    //60000 milliseconds in a minute
    exrS64 min = time / 60000;
    time = time - 60000 * min;

    //1000 milliseconds in a second
    exrS64 sec = time / 1000;
    time = time - 1000 * sec;

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

