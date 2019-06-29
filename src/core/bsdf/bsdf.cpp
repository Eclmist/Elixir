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

#include "bsdf.h"

exrBEGIN_NAMESPACE

void BSDF::AddComponent(const BxDF* bxdf)
{
	exrAssert(m_NumBxDF < MaxBxDFs, "Max number of BxDFs exceeded for material!");
	m_BxDFs[m_NumBxDF++] = bxdf;
}

exrU32 BSDF::GetComponentCount(BxDF::BxDFType flags) const
{
	exrU32 res = 0;
	for (exrU32 i = 0; i < m_NumBxDF; ++i)
		if (m_BxDFs[i]->MatchesFlags(flags)) ++res;

	return res;
}

exrEND_NAMESPACE