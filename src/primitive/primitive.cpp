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

#include "primitive.h"
#include "spatial/aabb.h"

exrBEGIN_NAMESPACE

AABB GeometricPrimitive::GetBoundingVolume() const
{
    return m_Shape->GetBoundingVolume();
}

exrBool GeometricPrimitive::Intersect(const Ray& ray, SurfaceInteraction* interaction) const
{
    exrFloat tHit;

    if (!m_Shape->Intersect(ray, &tHit, interaction)) return false;

    ray.m_TMax = tHit;
    interaction->m_Primitive = this;
}

exrBool GeometricPrimitive::HasIntersect(const Ray& r) const
{
    return m_Shape->HasIntersect(r);
}

exrEND_NAMESPACE
