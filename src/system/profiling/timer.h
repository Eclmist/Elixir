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