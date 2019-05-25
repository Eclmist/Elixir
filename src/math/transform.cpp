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

#include "transform.h"

exrBEGIN_NAMESPACE

void Transform::Translate(const exrVector3& delta)
{
    m_LocalMatrix.m_Data[3]  += delta.x;
    m_LocalMatrix.m_Data[7]  += delta.y;
    m_LocalMatrix.m_Data[11] += delta.z;

    m_InvLocalMatrix.m_Data[3]  -= delta.x;
    m_InvLocalMatrix.m_Data[7]  -= delta.y;
    m_InvLocalMatrix.m_Data[11] -= delta.z;
}

void Transform::SetTranslation(const exrVector3& translation)
{
    m_LocalMatrix.m_Data[3]  = translation.x;
    m_LocalMatrix.m_Data[7]  = translation.y;
    m_LocalMatrix.m_Data[11] = translation.z;

    m_InvLocalMatrix.m_Data[3]  = -translation.x;
    m_InvLocalMatrix.m_Data[7]  = -translation.y;
    m_InvLocalMatrix.m_Data[11] = -translation.z;
}

exrVector3 Transform::GetTranslation() const
{
    return exrVector3(m_LocalMatrix[3], m_LocalMatrix[7], m_LocalMatrix[11]);
}

void Transform::SetScale(const exrVector3& scale)
{
    m_LocalMatrix.m_Data[0]  = scale.x;
    m_LocalMatrix.m_Data[5]  = scale.y;
    m_LocalMatrix.m_Data[10] = scale.z;

    m_InvLocalMatrix.m_Data[0]  = 1.0f / scale.x;
    m_InvLocalMatrix.m_Data[5]  = 1.0f / scale.y;
    m_InvLocalMatrix.m_Data[10] = 1.0f / scale.z;
}

void Transform::Scale(const exrVector3& scale)
{
    m_LocalMatrix.m_Data[0]  *= scale.x;
    m_LocalMatrix.m_Data[5]  *= scale.y;
    m_LocalMatrix.m_Data[10] *= scale.z;

    m_InvLocalMatrix.m_Data[0]  /= scale.x;
    m_InvLocalMatrix.m_Data[5]  /= scale.y;
    m_InvLocalMatrix.m_Data[10] /= scale.z;
}

exrVector3 Transform::GetScale() const
{
    return exrVector3(m_LocalMatrix[0], m_LocalMatrix[5], m_LocalMatrix[10]);
}
exrEND_NAMESPACE
