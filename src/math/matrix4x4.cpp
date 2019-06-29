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

#include "matrix4x4.h"

exrBEGIN_NAMESPACE

Matrix4x4::Matrix4x4()
{
    m_Data[0]  = 1.0f; m_Data[1]  = 0.0f; m_Data[2]  = 0.0f; m_Data[3]  = 0.0f;
    m_Data[4]  = 0.0f; m_Data[5]  = 1.0f; m_Data[6]  = 0.0f; m_Data[7]  = 0.0f;
    m_Data[8]  = 0.0f; m_Data[9]  = 0.0f; m_Data[10] = 1.0f; m_Data[11] = 0.0f;
    m_Data[12] = 0.0f; m_Data[13] = 0.0f; m_Data[14] = 0.0f; m_Data[15] = 1.0f;
}

Matrix4x4::Matrix4x4(const exrFloat data[16])
{
    for (int i = 0; i < 16; ++i)
    {
        m_Data[i] = data[i];
    }
}

Matrix4x4::Matrix4x4(
    const exrFloat _11, const exrFloat _12, const exrFloat _13, const exrFloat _14,
    const exrFloat _21, const exrFloat _22, const exrFloat _23, const exrFloat _24,
    const exrFloat _31, const exrFloat _32, const exrFloat _33, const exrFloat _34,
    const exrFloat _41, const exrFloat _42, const exrFloat _43, const exrFloat _44)
{
    m_Data[0]  = _11; m_Data[1]  = _12; m_Data[2]  = _13; m_Data[3]  = _14;
    m_Data[4]  = _21; m_Data[5]  = _22; m_Data[6]  = _23; m_Data[7]  = _24;
    m_Data[8]  = _31; m_Data[9]  = _32; m_Data[10] = _33; m_Data[11] = _34;
    m_Data[12] = _41; m_Data[13] = _42; m_Data[14] = _43; m_Data[15] = _44;
}

Matrix4x4::Matrix4x4(const exrFloat data[4][4])
{
    for (int s = 0; s < 4; s++)
    {
        for (int t = 0; t < 4; t++)
        {
            m_Data2D[s][t] = data[s][t];
        }
    }
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& m) const
{
    exrFloat data[4][4];

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; j++) {
            exrFloat val = 0;
            for (int k = 0; k < 4; k++) {
                val += m_Data2D[i][k] * m.m_Data2D[k][j];
            }
            data[i][j] = val;
        }
    }

    return Matrix4x4(data);
}

Matrix4x4 Matrix4x4::Transposed() const
{
    return Matrix4x4(m_Data[0], m_Data[4], m_Data[8],  m_Data[12],
                     m_Data[1], m_Data[5], m_Data[9],  m_Data[13],
                     m_Data[2], m_Data[6], m_Data[10], m_Data[14],
                     m_Data[3], m_Data[7], m_Data[11], m_Data[15]);
}

Matrix4x4 Matrix4x4::Inversed() const
{
    float d = Determinant();
    if (fabs(d) < EXR_EPSILON)
        return Matrix4x4();

    // use Cramer's rule
    d = 1.0f / d;
    Matrix4x4 out;
    out.m_Data[0]  = d * (m_Data[5]  * (m_Data[10] * m_Data[15] - m_Data[11] * m_Data[14]) + m_Data[6]  * (m_Data[11] * m_Data[13] - m_Data[9]  * m_Data[15]) + m_Data[7]  * (m_Data[9]  * m_Data[14] - m_Data[10] * m_Data[13]));
    out.m_Data[1]  = d * (m_Data[9]  * (m_Data[2]  * m_Data[15] - m_Data[3]  * m_Data[14]) + m_Data[10] * (m_Data[3]  * m_Data[13] - m_Data[1]  * m_Data[15]) + m_Data[11] * (m_Data[1]  * m_Data[14] - m_Data[2]  * m_Data[13]));
    out.m_Data[2]  = d * (m_Data[13] * (m_Data[2]  * m_Data[7]  - m_Data[3]  * m_Data[6])  + m_Data[14] * (m_Data[3]  * m_Data[5]  - m_Data[1]  * m_Data[7])  + m_Data[15] * (m_Data[1]  * m_Data[6]  - m_Data[2]  * m_Data[5]));
    out.m_Data[3]  = d * (m_Data[1]  * (m_Data[7]  * m_Data[10] - m_Data[6]  * m_Data[11]) + m_Data[2]  * (m_Data[5]  * m_Data[11] - m_Data[7]  * m_Data[9])  + m_Data[3]  * (m_Data[6]  * m_Data[9]  - m_Data[5]  * m_Data[10]));
    out.m_Data[4]  = d * (m_Data[6]  * (m_Data[8]  * m_Data[15] - m_Data[11] * m_Data[12]) + m_Data[7]  * (m_Data[10] * m_Data[12] - m_Data[8]  * m_Data[14]) + m_Data[4]  * (m_Data[11] * m_Data[14] - m_Data[10] * m_Data[15]));
    out.m_Data[5]  = d * (m_Data[10] * (m_Data[0]  * m_Data[15] - m_Data[3]  * m_Data[12]) + m_Data[11] * (m_Data[2]  * m_Data[12] - m_Data[0]  * m_Data[14]) + m_Data[8]  * (m_Data[3]  * m_Data[14] - m_Data[2]  * m_Data[15]));
    out.m_Data[6]  = d * (m_Data[14] * (m_Data[0]  * m_Data[7]  - m_Data[3]  * m_Data[4])  + m_Data[15] * (m_Data[2]  * m_Data[4]  - m_Data[0]  * m_Data[6])  + m_Data[12] * (m_Data[3]  * m_Data[6]  - m_Data[2]  * m_Data[7]));
    out.m_Data[7]  = d * (m_Data[2]  * (m_Data[7]  * m_Data[8]  - m_Data[4]  * m_Data[11]) + m_Data[3]  * (m_Data[4]  * m_Data[10] - m_Data[6]  * m_Data[8])  + m_Data[0]  * (m_Data[6]  * m_Data[11] - m_Data[7]  * m_Data[10]));
    out.m_Data[8]  = d * (m_Data[7]  * (m_Data[8]  * m_Data[13] - m_Data[9]  * m_Data[12]) + m_Data[4]  * (m_Data[9]  * m_Data[15] - m_Data[11] * m_Data[13]) + m_Data[5]  * (m_Data[11] * m_Data[12] - m_Data[8]  * m_Data[15]));
    out.m_Data[9]  = d * (m_Data[11] * (m_Data[0]  * m_Data[13] - m_Data[1]  * m_Data[12]) + m_Data[8]  * (m_Data[1]  * m_Data[15] - m_Data[3]  * m_Data[13]) + m_Data[9]  * (m_Data[3]  * m_Data[12] - m_Data[0]  * m_Data[15]));
    out.m_Data[10] = d * (m_Data[15] * (m_Data[0]  * m_Data[5]  - m_Data[1]  * m_Data[4])  + m_Data[12] * (m_Data[1]  * m_Data[7]  - m_Data[3]  * m_Data[5])  + m_Data[13] * (m_Data[3]  * m_Data[4]  - m_Data[0]  * m_Data[7]));
    out.m_Data[11] = d * (m_Data[3]  * (m_Data[5]  * m_Data[8]  - m_Data[4]  * m_Data[9])  + m_Data[0]  * (m_Data[7]  * m_Data[9]  - m_Data[5]  * m_Data[11]) + m_Data[1]  * (m_Data[4]  * m_Data[11] - m_Data[7]  * m_Data[8]));
    out.m_Data[12] = d * (m_Data[4]  * (m_Data[10] * m_Data[13] - m_Data[9]  * m_Data[14]) + m_Data[5]  * (m_Data[8]  * m_Data[14] - m_Data[10] * m_Data[12]) + m_Data[6]  * (m_Data[9]  * m_Data[12] - m_Data[8]  * m_Data[13]));
    out.m_Data[13] = d * (m_Data[8]  * (m_Data[2]  * m_Data[13] - m_Data[1]  * m_Data[14]) + m_Data[9]  * (m_Data[0]  * m_Data[14] - m_Data[2]  * m_Data[12]) + m_Data[10] * (m_Data[1]  * m_Data[12] - m_Data[0]  * m_Data[13]));
    out.m_Data[14] = d * (m_Data[12] * (m_Data[2]  * m_Data[5]  - m_Data[1]  * m_Data[6])  + m_Data[13] * (m_Data[0]  * m_Data[6]  - m_Data[2]  * m_Data[4])  + m_Data[14] * (m_Data[1]  * m_Data[4]  - m_Data[0]  * m_Data[5]));
    out.m_Data[15] = d * (m_Data[0]  * (m_Data[5]  * m_Data[10] - m_Data[6]  * m_Data[9])  + m_Data[1]  * (m_Data[6]  * m_Data[8]  - m_Data[4]  * m_Data[10]) + m_Data[2]  * (m_Data[4]  * m_Data[9]  - m_Data[5]  * m_Data[8]));

    return out;
}

exrFloat Matrix4x4::Determinant() const
{
    return (m_Data[0] * m_Data[5] - m_Data[1] * m_Data[4]) * (m_Data[10] * m_Data[15] - m_Data[11] * m_Data[14]) -
           (m_Data[0] * m_Data[6] - m_Data[2] * m_Data[4]) * (m_Data[9]  * m_Data[15] - m_Data[11] * m_Data[13]) +
           (m_Data[0] * m_Data[7] - m_Data[3] * m_Data[4]) * (m_Data[9]  * m_Data[14] - m_Data[10] * m_Data[13]) +
           (m_Data[1] * m_Data[6] - m_Data[2] * m_Data[5]) * (m_Data[8]  * m_Data[15] - m_Data[11] * m_Data[12]) -
           (m_Data[1] * m_Data[7] - m_Data[3] * m_Data[5]) * (m_Data[8]  * m_Data[14] - m_Data[10] * m_Data[12]) +
           (m_Data[2] * m_Data[7] - m_Data[3] * m_Data[6]) * (m_Data[8]  * m_Data[13] - m_Data[9]  * m_Data[12]);
}

exrEND_NAMESPACE
