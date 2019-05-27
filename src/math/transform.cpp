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
    Matrix4x4 translationMat;
    translationMat.m_Data[3]  = delta.x;
    translationMat.m_Data[7]  = delta.y;
    translationMat.m_Data[11] = delta.z;

    m_TranslationMatrix = m_TranslationMatrix * translationMat;
    RecomputeGlobalMatrix();
}

void Transform::SetTranslation(const exrVector3& translation)
{
    m_TranslationMatrix.m_Data[3]  = translation.x;
    m_TranslationMatrix.m_Data[7]  = translation.y;
    m_TranslationMatrix.m_Data[11] = translation.z;

    RecomputeGlobalMatrix();
}

exrVector3 Transform::GetTranslation() const
{
    return exrVector3(m_GlobalMatrix[3], m_GlobalMatrix[7], m_GlobalMatrix[11]);
}

void Transform::SetScale(const exrVector3& scale)
{    
    m_ScaleMatrix.m_Data[0] = scale.x;
    m_ScaleMatrix.m_Data[5] = scale.y;
    m_ScaleMatrix.m_Data[10] = scale.z;

    RecomputeGlobalMatrix();
}

void Transform::Scale(const exrVector3& scale)
{   
    Matrix4x4 scaleMat;
    scaleMat.m_Data[0]  = scale.x;
    scaleMat.m_Data[5]  = scale.y;
    scaleMat.m_Data[10] = scale.z;

    m_ScaleMatrix = scaleMat * m_ScaleMatrix;
    RecomputeGlobalMatrix();
}

exrVector3 Transform::GetScale() const
{
    return exrVector3(m_GlobalMatrix[0], m_GlobalMatrix[5], m_GlobalMatrix[10]);
}

void Transform::RecomputeGlobalMatrix()
{
    m_GlobalMatrix = m_ScaleMatrix * m_TranslationMatrix;
    //m_GlobalMatrix = m_TranslationMatrix * m_RotationMatrix * m_ScaleMatrix;
    m_InvGlobalMatrix = m_GlobalMatrix.Inversed();
}

exrEND_NAMESPACE
