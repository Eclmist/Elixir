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
#include "core/primitive/transform.h"

exrBEGIN_NAMESPACE

class Scene;

class Light
{
public:

    enum LightFlags
    {
        LIGHTFLAGS_DELTAPOSITION  = 1 << 0,
        LIGHTFLAGS_DELTADIRECTION = 1 << 1,
        LIGHTFLAGS_AREA           = 1 << 2,
        LIGHTFLAGS_INFINITE       = 1 << 3,
    };

    Light(const Transform& transform, exrU32 flags)
        : m_Transform(transform)
        , m_Flags(flags) {};

    inline exrBool IsDeltaLight() { return m_Flags & LightFlags::LIGHTFLAGS_DELTAPOSITION || m_Flags & LightFlags::LIGHTFLAGS_DELTADIRECTION; };

public:
    virtual void Preprocess(const Scene& scene) {};
    virtual exrSpectrum Sample_f(const Interaction& interaction, const exrPoint2& uv, exrVector3& wi, exrFloat& pdf) const = 0;
    virtual exrSpectrum Power() const = 0;

    // TODO: make this protected once we have proper way to trace shadow rays
    const Transform m_Transform;
protected:
    const exrU32 m_Flags;
};

exrEND_NAMESPACE