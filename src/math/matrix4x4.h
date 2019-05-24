#pragma once

#include "core/elixir.h"

exrBEGIN_NAMESPACE

//! @brief A class that defines a standard 4x4 matrix
class Matrix4x4
{
public:
    //! @brief Constructs an identity matrix
    Matrix4x4();

    //! @brief Constructs a matrix with a float array
    //! @param data             An array of 16 floats
    Matrix4x4(const exrFloat* data);

    //! @brief Constructs a matrix with 16 floats
    Matrix4x4(const exrFloat _11, const exrFloat _12, const exrFloat _13, const exrFloat _14,
              const exrFloat _21, const exrFloat _22, const exrFloat _23, const exrFloat _24,
              const exrFloat _31, const exrFloat _32, const exrFloat _33, const exrFloat _34,
              const exrFloat _41, const exrFloat _42, const exrFloat _43, const exrFloat _44);

    //! @brief Multiplies one matrix with another
    //! @param m                The rhs matrix of the matrix multiplication
    //! @return                 The resultant matrix by multiplying this * m
    Matrix4x4 operator*(const Matrix4x4& m) const;

    //! @brief Transforms a point by the current matrix
    //! @param p                The point to transform
    //! @return                 The transformed point p'
    exrPoint operator*(const exrPoint& p) const;

    //! @brief Transforms a point by the current matrix
    //! @param p                The point to transform
    //! @return                 The transformed point p'
    exrPoint operator()(const exrPoint& p) const { return operator*(p); };

    //! @brief Transforms a vector by the current matrix
    //! @param v                The vector to transform
    //! @return                 The transformed vector v'
    exrVector3 operator*(const exrVector3& v) const;

    //! @brief Transforms a vector by the current matrix
    //! @param v                The vector to transform
    //! @return                 The transformed vector v'
    exrVector3 operator()(const exrVector3& v) const { return operator*(v); };

    //! @brief Shortcut to access the data with the [] operator directly
    exrFloat operator[](const exrU32& i) const { return m_Data[i]; }

    //! @brief Returns the transpose the current matrix
    Matrix4x4 Transposed() const;

    //! @brief Returns the inverse of the current matrix
    Matrix4x4 Inversed() const;

    //! @brief Returns the determinant of the current matrix
    exrFloat Determinant() const;

public:
    //! Matrix data
    union 
    {
        struct { exrFloat m_Data[16]; };
        struct
        {
            exrFloat m_11, m_12, m_13, m_14;
            exrFloat m_21, m_22, m_23, m_24;
            exrFloat m_31, m_32, m_33, m_34;
            exrFloat m_41, m_42, m_43, m_44;
        };
    };
};

exrEND_NAMESPACE