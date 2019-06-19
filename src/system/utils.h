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

// To detect multiple namespace opening which might be caused by using namespace elixir usage.
// if you get an error similar to the one below, it probably means that a using namespace elixir; statement
// has been found in an include file used by the source file.
// error C2872: 'inElixirNamespace' : ambiguous symbol
// To help you find the source, you should activate the display of file inclusion in advanced C++ settings of the project.
#ifdef EXR_USE_NAMESPACE
enum { InElixirNamespace = false };
namespace elixir { enum { InElixirNamespace = true }; }
#define exrBEGIN_NAMESPACE      exrStaticAssertMsg(!InElixirNamespace, "elixir namespace not previously closed"); namespace elixir {
#define exrEND_NAMESPACE        } exrStaticAssertMsg(!InElixirNamespace, "elixir namespace not previously closed");
#else
#define exrBEGIN_NAMESPACE      ((void)0)
#define exrEND_NAMESPACE        ((void)0)
#endif

inline uint32_t FloatToBits(float f)
{
    uint32_t ui;
    memcpy(&ui, &f, sizeof(float));
    return ui;
}

inline float BitsToFloat(uint32_t ui)
{
    float f;
    memcpy(&f, &ui, sizeof(uint32_t));
    return f;
}
