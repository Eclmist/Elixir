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

#include <list>

exrBEGIN_NAMESPACE

#define EXR_L1_CACHE_LINE_SIZE 64   // Default cache line size of 64 bytes

// "Placement new" syntax - https://isocpp.org/wiki/faq/dtors#placement-new
#define EXR_ARENA_ALLOC(arena, Type) new ((arena).Allocate(sizeof(Type))) Type

class MemoryArena 
{
public:
    //! @brief Constructs a memory arena given a block size (default: 256kb)
    MemoryArena(size_t blockSize = 262144) : m_BlockSize(blockSize) {};

    ~MemoryArena() {
        FreeAligned(m_CurrentBlock);
        for (auto& block : m_UsedBlocks) FreeAligned(block.second);
        for (auto& block : m_AvailableBlocks) FreeAligned(block.second);
    }

    void* Allocate(size_t numBytes)
    {
        // Round up numBytes to minimum machine alignment
        const int align = 16;
        numBytes = ((numBytes + align - 1) & (~align - 1));

        // If the requested amount of memory does not fit into the current block,
        // dynamically allocate a new block.
        if (m_CurrentBlockPos + numBytes > m_CurrentAllocSize)
        {
            // If current block exist, set to used, since it is not big enough
            if (m_CurrentBlock)
            {
                m_UsedBlocks.emplace_back(m_CurrentAllocSize, m_CurrentBlock);
                m_CurrentBlock = nullptr;
                m_CurrentAllocSize = 0;
            }

            // Find an available block that can fit the size requested
            for (auto it = m_AvailableBlocks.begin(); it != m_AvailableBlocks.end(); ++it)
            {
                if (it->first >= numBytes)
                {
                    m_CurrentAllocSize = it->first;
                    m_CurrentBlock = it->second;
                    m_AvailableBlocks.erase(it);
                    break;
                }
            }

            // Cannot find any available blocks, so create a new block
            if (!m_CurrentBlock)
            {
                m_CurrentAllocSize = std::max(numBytes, m_BlockSize);
                m_CurrentBlock = (uint8_t*)AllocateAligned(m_CurrentAllocSize * sizeof(uint8_t));
            }

            m_CurrentBlockPos = 0;
        }

        void* ret = m_CurrentBlock + m_CurrentBlockPos;
        m_CurrentBlockPos += numBytes;
        return ret;
    }

    void Release()
    {
        m_CurrentBlockPos = 0;
        m_AvailableBlocks.splice(m_AvailableBlocks.begin(), m_UsedBlocks);
    }

    size_t TotalAllocated() const
    {
        size_t total = m_CurrentAllocSize;
        for (const auto& block : m_UsedBlocks) total += block.first;
        for (const auto& block : m_AvailableBlocks) total += block.first;
        return total;
    }

private:
    void* AllocateAligned(size_t size)
    {
#if defined (EXR_HAVE_ALIGNED_MALLOC)
        return _aligned_malloc(size, EXR_L1_CACHE_LINE_SIZE);
#elif defined (EXR_HAVE_POSIX_MEMALIGN)
        void* ptr;
        if (posix_memalign(&ptr, EXR_L1_CACHE_LINE_SIZE, size) != 0)
            ptr = nullptr;
        return ptr;
#else
        return memalign(EXR_L1_CACHE_LINE_SIZE, size);
#endif
    }

    void FreeAligned(void* ptr)
    {
        if (!ptr)
            return;
#if defined (EXR_HAVE_ALIGNED_MALLOC)
        _aligned_free(ptr);
#else
        free(ptr);
#endif
    }

private:
    const size_t m_BlockSize;
    size_t m_CurrentBlockPos = 0;
    size_t m_CurrentAllocSize = 0;
    uint8_t* m_CurrentBlock = nullptr;
    std::list<std::pair<size_t, uint8_t*>> m_UsedBlocks;
    std::list<std::pair<size_t, uint8_t*>> m_AvailableBlocks;
};

exrEND_NAMESPACE