#ifndef __CORE_SYSTEM_PROFILING_TIMER_H__
#define __CORE_SYSTEM_PROFILING_TIMER_H__

#include "core/elixir.h"

exrBEGIN_NAMESPACE
//! @brief A simple code timer implemented with ctime
//!
//! A code timer that can measure and print the time taken from object declaration to 
//! end of object scope
class Timer
{
public:
    //! @brief Create a timer with a name identifier
    //! @param processName      The name of the process being timed
    Timer(exrString processName = "Unamed Process");

    //! @brief Destructor that stops the timer. This should not be explicitly called
    ~Timer();

    //! @brief Stops the timer before the timer goes out of scope
    //!
    //! In the case that a process does not have its own scope or that it is beneficial to
    //! not have its own scope, this function can be used to stop the timer before the scope
    //! is closed.
    void EndTimer();

    //! @brief Formats a 64 bit time value into human readable time
    //!
    //! @param time             Input time to convert
    //! @param hh               Zero padded output hour string (00 - 24)
    //! @param mm               Zero padded output minute string (00 - 60)
    //! @param ss               Zero padded output second string (00 - 60)
    static void FormatTime(exrS64 time, exrString& hh, exrString& mm, exrString& ss);

    //! @brief Computes the number of milliseconds past since Epoch
    //!
    //! @return                 The number of milliseconds since 1970/01/01
    static exrU64 TimeSinceEpochMillisec();

private:
    //! Name of current process. Only used for logging.
    exrString m_ProcessName;

    //! Time when the timer first began ticking
    exrS64 m_StartTime;

    //! A flag that will be set by EndTimer() to prevent destructor from logging again
    exrBool m_HasEarlyExit;
};

exrEND_NAMESPACE

#endif // !__CORE_SYSTEM_PROFILING_TIMER_H__