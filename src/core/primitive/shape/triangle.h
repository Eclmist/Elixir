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

#include "shape.h"
#include "core/primitive/mesh.h"

exrBEGIN_NAMESPACE

class Triangle : public Shape
{
public:
    Triangle(const std::shared_ptr<Mesh>& mesh, exrU32 indices[3])
        : m_SharedMesh(mesh)
    {
        m_Indices[0] = indices[0];
        m_Indices[1] = indices[1];
        m_Indices[2] = indices[2];
    };

    exrBool Intersect(const Ray& ray, exrFloat& tHit, SurfaceInteraction* interaction) const override;
    exrBool HasIntersect(const Ray& ray, exrFloat& tHit) const override;

protected:
    AABB ComputeBoundingVolume() const override;

private:
    std::shared_ptr<Mesh> m_SharedMesh;
    exrU32 m_Indices[3];
};

exrEND_NAMESPACE