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

#include "samplerintegrator.h"

exrBEGIN_NAMESPACE

void SamplerIntegrator::Render(const Scene& scene)
{
    Preprocess(scene);

    Film* film = m_Camera->m_Film.get();

    // Compute number of tiles
    Point2<exrU32> maxResolution = film->m_Resolution;
    Point2<exrU32> numTiles((maxResolution.x + TileSize - 1) / TileSize,
                            (maxResolution.y + TileSize - 1) / TileSize);

    exrU32 totalNumTiles = numTiles.x * numTiles.y;

    // Loop in terms of x,y tiles so this can become async in the future
    for (exrU32 i = 0; i < totalNumTiles; ++i)
    {
        exrU32 tileX = i % numTiles.x;
        exrU32 tileY = exrU32(i / exrFloat(numTiles.x));

        // Compute bounds for tile
        Point2<exrU32> tileMin(tileX * TileSize, tileY * TileSize);
        Point2<exrU32> tileMax(tileX * TileSize + TileSize, tileY * TileSize + TileSize);

        // Foreach pixel, shade
        for (exrU32 x = 0; x < TileSize; ++x)
        {
            for (exrU32 y = 0; y < TileSize; ++y)
            {
                // Foreach sample
                for (exrU32 n = 0; n < m_NumSamplesPerPixel; ++n)
                {
                    exrFloat u = exrFloat(tileMin.x + x + Random::Uniform01()) / exrFloat(maxResolution.x);
                    exrFloat v = exrFloat(tileMin.y + y + Random::Uniform01()) / exrFloat(maxResolution.y);
                    RayDifferential viewRay = m_Camera->GetViewRay(u, v);
                    
                    exrSpectrum L(0.0f);
                    L += Evaluate(viewRay, scene);

                    film->AddSplat(Point2<exrU32>(tileMin.x + x, tileMin.y + y), L);
                }
            }
        }

    }

    film->WriteImage(1.0f / m_NumSamplesPerPixel);
}


exrEND_NAMESPACE
