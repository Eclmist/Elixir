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

#include "primitive.h"

exrBEGIN_NAMESPACE

struct Vertex
{
    exrPoint3 m_Position;
    exrVector3 m_Normal;
    exrVector2 m_TexCoord;
};

//! A class that stores all the information that triangles in a mesh might need
//! access to. This allows multiple triangles in a mesh to share the same vertex
//! and reduce the overall amount of memory required to store a mesh in memory.
class Mesh
{
public:
    static Mesh LoadFromFile(const exrChar* fileName);

    const exrBool GetVertexAtIndex(exrU32 faceIndex, Vertex& v1, Vertex& v2, Vertex& v3) const;

    exrU32 m_NumVertices = 0;
    exrU32 m_NumFaces = 0;

private:
    std::vector<exrU32> m_IndexBuffer;
    std::vector<exrPoint3> m_PositionBuffer;
    std::vector<exrVector2> m_TexCoordBuffer;
    std::vector<exrVector3> m_NormalBuffer;
};

exrEND_NAMESPACE