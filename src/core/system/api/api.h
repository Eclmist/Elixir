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

/*
    API function declarations
    Make sure to increment API version when there are changes
    Format: Major.Minor.Patch-PreleaseID
    For more versioning information, see <https://semver.org/>
*/
#define EXR_VERSION_MAJOR 0
#define EXR_VERSION_MINOR 1
#define EXR_VERSION_PATCH 0
#define EXR_VERSION_PRERELEASEID "dev" 

exrBEGIN_NAMESPACE

void ElixirInit(const Options& options);
void ElixirCleanup();

void ElixirParseFile(const exrString& filename);
exrEND_NAMESPACE