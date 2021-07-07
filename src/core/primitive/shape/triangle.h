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

#pragma once

#include "shape.h"
#include "core/primitive/mesh.h"

exrBEGIN_NAMESPACE

class Triangle : public Shape
{
public:
    Triangle(const std::shared_ptr<Mesh>& mesh, exrU32 index)
        : m_SharedMesh(mesh)
        , m_IndexInMesh(index) {};

    exrBool Intersect(const Ray& ray, exrFloat& tHit, SurfaceInteraction* interaction) const override;
    exrBool HasIntersect(const Ray& ray, exrFloat& tHit) const override;

protected:
    AABB ComputeBoundingVolume() const override;

private:
    std::shared_ptr<Mesh> m_SharedMesh;
    exrU32 m_IndexInMesh;
};

exrEND_NAMESPACE