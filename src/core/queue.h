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

#include <queue>
#include "core/elixir.h"

exrBEGIN_NAMESPACE

//! @brief A thread-safe queue
//! 
//! Provides a wrapper around a basic queue to provide thread safety
template <class T>
class Queue
{
public:
    //! @param ray              The ray to test against

    //! @brief Destructor that invalidates the queue
    ~Queue()
    {
        Invalidate();
    }
    
    //! Push a new item onto the queue
    //! @param value            The item to push
    void Push(T value)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Queue.push(std::move(value));
        m_Condition.notify_one();
    }

    //! Attempt to get the first value in the queue
    //!
    //! @param out              The item that was popped
    //! @return                 True if a value was popped
    exrBool TryPop(T& out)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if (m_Queue.empty() || !m_Valid)
        {
            return false;
        }
         
        out = std::move(m_Queue.front());
        m_Queue.pop();
        return true;
    }

    //! @brief Pop the first value in the queue, even if it is blocking.
    //!
    //! @param out              The item that was popped
    //! @return                 True if a value was popped
    exrBool WaitPop(T& out)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);

        m_Condition.wait(lock, [this]()
        {
            return !m_Queue.empty() || !m_Valid;
        });

        if (!m_Valid)
        {
            return false;
        }

        out = std::move(m_Queue.front());
        m_Queue.pop();
        return true;
    }

    //! Clears all items from the queue
    void Clear()
    {
        std::lock_guard<std::mutex> lock(m_Mutex);

        while (!m_Queue.empty())
        {
            m_Queue.pop();
        }

        m_Condition.notify_all();
    }

    //! @brief Invalidates the queue
    //!
    //! Used to ensure that no conditions are still waiting in waitPop
    //! when a thread of the application is trying to exit. The queue is invalid
    //! after calling this method and it is an error to continue using the queue
    //! after this method has been called.
    void Invalidate()
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Valid = false;
        m_Condition.notify_all();
    }

    //! Returns if the queue is empty
    exrBool IsEmpty() const
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        return m_Queue.empty();
    }

    //! Returns if the queue is still valid
    exrBool IsValid() const
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        return m_Valid;
    }

private:
    std::atomic_bool m_Valid = true;
    mutable std::mutex m_Mutex;
    std::queue<T> m_Queue;
    std::condition_variable m_Condition;
};

exrEND_NAMESPACE