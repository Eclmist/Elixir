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

#include "mesh.h"

exrBEGIN_NAMESPACE

Mesh::Mesh(std::vector<exrU32>& indices, std::vector<Vertex>& vertices)
    : m_IndexBuffer(indices)
    , m_VertexBuffer(vertices)
{
    exrAssert(indices.size() % 3 == 0, "Index array is not a multiple 3!");
}

exrU32 Mesh::GetNumTriangle() const
{
    return (exrU32)m_IndexBuffer.size() % 3;
}

exrU32 Mesh::GetNumVertices() const
{
    return (exrU32)m_VertexBuffer.size();
}

const Vertex* Mesh::GetVertexAtIndex(exrU32 index) const
{
    if (index >= m_VertexBuffer.size())
        return nullptr;

    return &m_VertexBuffer[index];
}

exrEND_NAMESPACE

