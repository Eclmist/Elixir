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
        : m_LocalMatrix(Matrix4x4::Identity())
        , m_InvLocalMatrix(Matrix4x4::Identity()) {};

    //! @brief Creates a transform with a local matrix, and contructs an inverse matrix
    Transform(const Matrix4x4& matrix)
        : m_LocalMatrix(matrix)
        , m_InvLocalMatrix(matrix.Inversed()) {};

    //! @brief Creates a transform with a local matrix and a inverse local matrix
    Transform(const Matrix4x4& matrix, const Matrix4x4& inverseMatrix)
        : m_LocalMatrix(matrix)
        , m_InvLocalMatrix(inverseMatrix) {};

    //! @brief Translate the current transform
    //! @param delta            The translation vector
    void Translate(const exrVector3& delta);

    //! @brief Sets the translation the current transform
    //! @param translation      The translation vector
    void SetTranslation(const exrVector3& translation);

    //! @brief Returns the translation of the current transform
    exrVector3 GetTranslation() const;

    //! @brief Scales the current transform
    //! @param scale            The scale amount
    void Scale(const exrVector3& scale);

    //! @brief Sets the scale the current transform
    //! @param scale            The scale vector
    void SetScale(const exrVector3& scale);

    //! @brief Returns the scale of the current transform
    exrVector3 GetScale() const;

    //! @brief Rotates the current transform
    //! @param rotation         ??????????????????????
    void Rotate(const exrVector3& scale);

public:
    //! @brief Returns the current local matrix
    const Matrix4x4& GetMatrix() const { return m_LocalMatrix; }

    //! @brief Returns the inverse of the current local matrix
    const Matrix4x4& GetInverseMatrix() const { return m_InvLocalMatrix; }

private:
    //! The local matrix
    Matrix4x4 m_LocalMatrix;

    //! The inverse local matrix
    Matrix4x4 m_InvLocalMatrix;
};

exrEND_NAMESPACE