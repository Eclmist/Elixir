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

exrBEGIN_NAMESPACE

class ProgressBar
{
public:
    ProgressBar(int maxVal, int barLength)
        : m_MaxValue(maxVal) 
        , m_BarLength(barLength)
        , m_StartTime(Timer::TimeSinceEpochMillisec()) {};

    void Increment(int value) 
    {
        m_CurrentValue.fetch_add(value);
    }

    void Print() 
    {
        if (m_IsUpdating)
            return;

        m_IsUpdating = true;

        std::cout << "[Info]\t   [";
        float progress = m_CurrentValue / static_cast<float>(m_MaxValue);

        for (int i = 0; i < m_BarLength; ++i) 
        {
            float barProgress = i / static_cast<float>(m_BarLength);
            if (barProgress <= progress)
                std::cout << "#";
            else
                std::cout << " ";
        }

        std::cout << "] " << static_cast<int>(progress * 100) << "% ";

        auto endTime = Timer::TimeSinceEpochMillisec();
        auto timeLeftMilli = ((endTime - m_StartTime) / m_CurrentValue) * (m_MaxValue - m_CurrentValue);

        std::string hh, mm, ss;
        Timer::FormatTime(static_cast<long>(timeLeftMilli), hh, mm, ss);

        std::cout << "- Estimated Time Remaining: " << hh << ":" << mm << ":" << ss << "\t";

        if (progress < 1)
            std::cout << "\r";

        std::cout << std::flush;

        m_IsUpdating = false;
    }

private:
    int m_MaxValue;
    int m_BarLength;
    unsigned long m_StartTime;
    bool m_HasUpdated;
    std::atomic<bool> m_IsUpdating;
    std::atomic<int> m_CurrentValue;
};

exrEND_NAMESPACE