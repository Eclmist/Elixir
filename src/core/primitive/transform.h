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

exrBEGIN_NAMESPACE

//! @brief A class that handles linear transformations of points and vectors
//!
//! The transform class handles translations, rotations, coordinate space convertions
//! using internal matrix multiplications
class Transform
{
public:
    
    //! @brief Creates a transform with identity matrices
    Transform()
        : m_GlobalMatrix(Matrix4x4::Identity())
        , m_InvGlobalMatrix(Matrix4x4::Identity()) {};

    //! @brief Copy constructor
    Transform(const Transform& copy)
        : m_GlobalMatrix(copy.m_GlobalMatrix)
        , m_InvGlobalMatrix(copy.m_InvGlobalMatrix) {};

    //! @brief Translate the current transform
    //! @param delta            The translation vector
    void Translate(const exrVector3& delta);

    //! @brief Sets the translation of the current transform
    //! @param translation      The translation vector
    void SetTranslation(const exrVector3& translation);

    //! @brief Returns the translation of the current transform
    exrVector3 GetTranslation() const;

    //! @brief Scales the current transform
    //! @param scale            The scale amount
    void Scale(const exrFloat scale);

    //! @brief Sets the scale of the current transform
    //! @param scale            The scale amount
    void SetScale(const exrFloat scale);

    //! @brief Returns the scale of the current transform
    exrFloat GetScale() const;

    //! @brief Rotates the current transform
    //! @param rotation         The Euler rotation vector
    void Rotate(const exrVector3& rotation);
    
    //! @brief Sets the rotation of the current transform
    //! @param rotation         The Euler rotation vector
    void SetRotation(const exrVector3& rotation);

public:
    //! @brief Returns the current local matrix
    const Matrix4x4& GetMatrix() const { return m_GlobalMatrix; }

    //! @brief Returns the inverse of the current local matrix
    const Matrix4x4& GetInverseMatrix() const { return m_InvGlobalMatrix; }

    //! @brief Returns the final position of the of the current transform applied to origin
    exrPoint3 GetPosition() const { return m_GlobalMatrix * exrPoint3::Zero(); }

private:
    void RecomputeGlobalMatrix();

private:
    //! The translation matrix
    Matrix4x4 m_TranslationMatrix;

    //! The scale matrix
    Matrix4x4 m_ScaleMatrix;

    //! The rotation matrices
    Matrix4x4 m_RotationMatrixX;
    Matrix4x4 m_RotationMatrixY;
    Matrix4x4 m_RotationMatrixZ;
    exrVector3 m_EulerAngles;

    //! The global matrix
    Matrix4x4 m_GlobalMatrix;
    //! The inverse global matrix
    Matrix4x4 m_InvGlobalMatrix;
};

exrEND_NAMESPACE