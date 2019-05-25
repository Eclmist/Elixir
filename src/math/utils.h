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


/* ========================================================================================== /

         ######\  ##\      ########\  ######\  ##\   ##\       ##\      ##\ ########\
        ##  __##\ ## |     ##  _____|##  __##\ ###\  ## |      ###\    ### |##  _____|
        ## /  \__|## |     ## |      ## /  ## |####\ ## |      ####\  #### |## |
        ## |      ## |     #####\    ######## |## ##\## |      ##\##\## ## |#####\
        ## |      ## |     ##  __|   ##  __## |## \#### |      ## \###  ## |##  __|
        ## |  ##\ ## |     ## |      ## |  ## |## |\### |      ## |\#  /## |## |
        \######  |########\########\ ## |  ## |## | \## |      ## | \_/ ## |########\
         \______/ \________\________|\__|  \__|\__|  \__|      \__|     \__|\________|

             Banner of shame. Remove when this file is no longer ghetto.
 =========================================================================================== */


// This file should probably be merged into math/math.h
#pragma once

#include "core/elixir.h"

exrBEGIN_NAMESPACE

exrFloat SchlickFresnelApproximation(exrFloat cosine, exrFloat ior)
{
    exrFloat r0 = (1.0f - ior) / (1.0f + ior);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * exrPow((1.0f - cosine), 5);
}

exrEND_NAMESPACE