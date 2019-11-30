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
#include <sstream>
#include <fstream>

exrBEGIN_NAMESPACE

Mesh Mesh::LoadFromFile(const exrChar* fileName)
{
    Mesh mesh;
    std::ifstream infile(fileName);
    exrAssert(infile, "Input file does not exist!");

    std::string line;
    while (getline(infile, line))
    {
        std::replace(line.begin(), line.end(), '\/', ' ');
        if (line.substr(0, 2) == "v ")
        {
            std::istringstream iss(line.substr(2));
            exrPoint3 v;
            iss >> v.x;
            iss >> v.y;
            iss >> v.z;

            mesh.m_PositionBuffer.push_back(v);
            mesh.m_NumVertices++;
        }
        else if (line.substr(0, 2) == "vt")
        {
            std::istringstream iss(line.substr(2));
            exrVector2 t;
            iss >> t.x;
            iss >> t.y;

            mesh.m_TexCoordBuffer.push_back(t);
        }
        else if (line.substr(0, 2) == "vn")
        {
            std::istringstream iss(line.substr(2));
            exrVector3 n;
            iss >> n.x;
            iss >> n.y;
            iss >> n.z;

            mesh.m_NormalBuffer.push_back(n);
        }
        else if (line.substr(0, 2) == "f ")
        {
            exrU32 x1, t1, n1, x2, t2, n2, x3, t3, n3;

            std::istringstream iss(line.substr(2));

            iss >> x1; iss >> t1; iss >> n1;
            iss >> x2; iss >> t2; iss >> n2;
            iss >> x3; iss >> t3; iss >> n3;

            x1--; t1--; n1--;
            x2--; t2--; n2--;
            x3--; t3--; n3--;

            mesh.m_IndexBuffer.push_back(x1);
            mesh.m_IndexBuffer.push_back(t1);
            mesh.m_IndexBuffer.push_back(n1);

            mesh.m_IndexBuffer.push_back(x2);
            mesh.m_IndexBuffer.push_back(t2);
            mesh.m_IndexBuffer.push_back(n2);

            mesh.m_IndexBuffer.push_back(x3);
            mesh.m_IndexBuffer.push_back(t3);
            mesh.m_IndexBuffer.push_back(n3);

            mesh.m_NumFaces++;
        }
    }

    return mesh;
}

const exrBool Mesh::GetVertexAtIndex(exrU32 faceIndex, Vertex& v1, Vertex& v2, Vertex& v3) const
{
    if (faceIndex * 9 + 8 >= m_IndexBuffer.size())
        return false;


    v1.m_Position = m_PositionBuffer[m_IndexBuffer[faceIndex * 9]];
    v1.m_TexCoord = m_TexCoordBuffer[m_IndexBuffer[faceIndex * 9 + 1]];
    v1.m_Normal = m_NormalBuffer[m_IndexBuffer[faceIndex * 9 + 2]];

    v2.m_Position = m_PositionBuffer[m_IndexBuffer[faceIndex * 9 + 3]];
    v2.m_TexCoord = m_TexCoordBuffer[m_IndexBuffer[faceIndex * 9 + 4]];
    v2.m_Normal = m_NormalBuffer[m_IndexBuffer[faceIndex * 9 + 5]];

    v3.m_Position = m_PositionBuffer[m_IndexBuffer[faceIndex * 9 + 6]];
    v3.m_TexCoord = m_TexCoordBuffer[m_IndexBuffer[faceIndex * 9 + 7]];
    v3.m_Normal = m_NormalBuffer[m_IndexBuffer[faceIndex * 9 + 8]];

    return true;
}

exrEND_NAMESPACE

