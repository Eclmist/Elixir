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

#include "system/system.h"
#include <sstream>
#include <iomanip>

exrBEGIN_NAMESPACE

#ifdef EXR_PLATFORM_WIN
#define PROGRESSBAR_BLOCK_CHAR         std::string("#")
#define PROGRESSBAR_BOX_CHAR           std::string("-")
#define PROGRESSBAR_LEFTHANDLE_CHAR    std::string("[")
#define PROGRESSBAR_RIGHTHANDLE_CHAR   std::string("]")
#else
#define PROGRESSBAR_BLOCK_CHAR         std::string("█")
#define PROGRESSBAR_BOX_CHAR           std::string("░")
#define PROGRESSBAR_LEFTHANDLE_CHAR    std::string("")
#define PROGRESSBAR_RIGHTHANDLE_CHAR   std::string("")
#endif

class ProgressBar
{
public:
    ProgressBar(int maxVal, int barLength)
        : m_MaxValue(maxVal) 
        , m_BarLength(barLength)
        , m_StartTime(Timer::TimeSinceEpochMillisec()) { };

    void Increment(int value) 
    {
        m_CurrentValue.fetch_add(value);
    }

    void Print() 
    {
        if (m_IsUpdating)
            return;

        m_IsUpdating = true;

        std::string progString = "[Info]\t   " + PROGRESSBAR_LEFTHANDLE_CHAR;
        float progress = m_CurrentValue / static_cast<float>(m_MaxValue);

        for (int i = 0; i < m_BarLength; ++i) 
        {
            float barProgress = i / static_cast<float>(m_BarLength);
            if (barProgress <= progress)
                progString += PROGRESSBAR_BLOCK_CHAR;
            else
                progString += PROGRESSBAR_BOX_CHAR;
        }

        std::stringstream progressStr;
        progressStr << std::fixed << std::setprecision(2) << (progress * 100);
        progString += PROGRESSBAR_RIGHTHANDLE_CHAR + " " + progressStr.str() + "% ";

        auto endTime = Timer::TimeSinceEpochMillisec();
        auto timeLeftMilli = ((endTime - m_StartTime) / m_CurrentValue) * (m_MaxValue - m_CurrentValue);

        std::string hh, mm, ss;
        Timer::FormatTime(static_cast<long>(timeLeftMilli), hh, mm, ss);

        progString += "- Estimated Time Remaining: " + hh + ":" + mm + ":" + ss + "\t\r";

        std::cout << progString << std::flush;

        m_IsUpdating = false;
    }

private:
    int m_MaxValue;
    int m_BarLength;
    unsigned long m_StartTime;
    std::atomic<bool> m_IsUpdating = false;
    std::atomic<int> m_CurrentValue = 0;
};

exrEND_NAMESPACE