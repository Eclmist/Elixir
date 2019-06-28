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

#include "integrator.h"

exrBEGIN_NAMESPACE

class SamplerIntegrator : public Integrator
{
public:

    SamplerIntegrator(std::unique_ptr<Camera> camera, std::unique_ptr<Sampler> sampler)
        : m_Camera(camera)
        , m_Sampler(sampler)

    virtual void Render(const Scene& scene) override;
    virtual void Preprocess(const Scene& scene) {}

protected:

private:
    std::unique_ptr<Sampler> m_Sampler;
    std::unique_ptr<Camera> m_Camera;    
};


exrEND_NAMESPACE