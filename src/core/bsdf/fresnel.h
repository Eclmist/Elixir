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

class Fresnel
{
public:
    virtual exrSpectrum Evaluate(exrFloat cosThetaI) const = 0;

    static exrFloat FrDielectric(exrFloat cosThetaI, exrFloat etaI, exrFloat etaT);

    static exrFloat FrConductor(exrFloat cosThetaI, const exrSpectrum& etaI, const exrSpectrum& etaT,
        const exrSpectrum& k);

    static exrSpectrum FrSchlick(exrSpectrum f0, exrFloat vDotH);
};

class FresnelDielectric : public Fresnel
{
public:
    FresnelDielectric(exrFloat etaI, exrFloat etaT)
        : m_EtaI(etaI)
        , m_EtaT(etaT) {};

    exrSpectrum Evaluate(exrFloat cosThetaI) const override;

private:
    exrFloat m_EtaI;
    exrFloat m_EtaT;
};

exrEND_NAMESPACE