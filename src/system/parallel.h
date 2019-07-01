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

exrBEGIN_NAMESPACE

class AtomicFloat
{
public:
    explicit AtomicFloat(exrFloat v = 0) { m_Bits = FloatToBits(v); }
    operator exrFloat() const { return BitsToFloat(m_Bits); }
    exrFloat operator=(exrFloat v)
    {
        m_Bits = FloatToBits(v);
        return v;
    }
    void Add(exrFloat v)
    {
        exrU32 oldBits = m_Bits, newBits;

        do
        {
            newBits = FloatToBits(BitsToFloat(oldBits) + v);
        } while (!m_Bits.compare_exchange_weak(oldBits, newBits));
    }

private:
    std::atomic<exrU32> m_Bits;
};


exrEND_NAMESPACE
