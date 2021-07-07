/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019-2020 Samuel Van Allen - All rights reserved.

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
    translationMat.m_14  = delta.x;
    translationMat.m_24 = delta.y;
    translationMat.m_34 = delta.z;

    m_TranslationMatrix = m_TranslationMatrix * translationMat;
    RecomputeGlobalMatrix();
}

void Transform::SetTranslation(const exrVector3& translation)
{
    m_TranslationMatrix.m_14  = translation.x;
    m_TranslationMatrix.m_24  = translation.y;
    m_TranslationMatrix.m_34 = translation.z;

    RecomputeGlobalMatrix();
}

exrVector3 Transform::GetTranslation() const
{
    return exrVector3(m_GlobalMatrix.m_14, m_GlobalMatrix.m_24, m_GlobalMatrix.m_34);
}

void Transform::SetScale(const exrFloat scale)
{    
    m_ScaleMatrix.m_11 = scale;
    m_ScaleMatrix.m_22 = scale;
    m_ScaleMatrix.m_33 = scale;

    RecomputeGlobalMatrix();
}

void Transform::Scale(const exrFloat scale)
{   
    Matrix4x4 scaleMat;
    scaleMat.m_11  = scale;
    scaleMat.m_22  = scale;
    scaleMat.m_33 = scale;

    m_ScaleMatrix = scaleMat * m_ScaleMatrix;
    RecomputeGlobalMatrix();
}

exrFloat Transform::GetScale() const
{
    exrAssert(m_GlobalMatrix.m_11 == m_GlobalMatrix.m_22 && m_GlobalMatrix.m_22 == m_GlobalMatrix.m_33, "Only uniform scaling is supported, but scales matrix is non-uniform!");
    return m_GlobalMatrix.m_11;
}

void Transform::Rotate(const exrVector3& rotation)
{
    if (rotation.x != 0)
    {
        Matrix4x4 rotX;
        rotX.m_21 += cos(rotation.x);
        rotX.m_22 += sin(rotation.x);
        rotX.m_31 += -sin(rotation.x);
        rotX.m_32 += cos(rotation.x);
        m_RotationMatrixX = rotX * m_RotationMatrixX;
    }

    if (rotation.y != 0)
    {
        Matrix4x4 rotY;
        rotY.m_11 = cos(rotation.y);
        rotY.m_13 = -sin(rotation.y);
        rotY.m_31 = sin(rotation.y);
        rotY.m_33 = cos(rotation.y);
        m_RotationMatrixY = rotY * m_RotationMatrixY;
    }

    if (rotation.z != 0)
    {
        Matrix4x4 rotZ;
        rotZ.m_11 = cos(rotation.z);
        rotZ.m_12 = -sin(rotation.z);
        rotZ.m_21 = sin(rotation.z);
        rotZ.m_22 = cos(rotation.x);
        m_RotationMatrixZ = rotZ * m_RotationMatrixZ;
    }

    RecomputeGlobalMatrix();
}

void Transform::SetRotation(const exrVector3& rotation)
{
    m_RotationMatrixX.m_22 = cos(rotation.x);
    m_RotationMatrixX.m_23 = -sin(rotation.x);
    m_RotationMatrixX.m_32 = sin(rotation.x);
    m_RotationMatrixX.m_33 = cos(rotation.x);

    m_RotationMatrixY.m_11 = cos(rotation.y);
    m_RotationMatrixY.m_13 = sin(rotation.y);
    m_RotationMatrixY.m_31 = -sin(rotation.y);
    m_RotationMatrixY.m_33 = cos(rotation.y);

    m_RotationMatrixZ.m_11 = cos(rotation.z);
    m_RotationMatrixZ.m_12 = -sin(rotation.z);
    m_RotationMatrixZ.m_21 = sin(rotation.z);
    m_RotationMatrixZ.m_22 = cos(rotation.z);

    RecomputeGlobalMatrix();
}

void Transform::RecomputeGlobalMatrix()
{
    m_GlobalMatrix = m_TranslationMatrix * m_RotationMatrixZ * m_RotationMatrixX * m_RotationMatrixY * m_ScaleMatrix;
    m_InvGlobalMatrix = m_GlobalMatrix.Inversed();
}

exrEND_NAMESPACE
