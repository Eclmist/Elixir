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

//! @brief A base class for different bidirectional distribution functions.
//!
//! Handles the basic operations that all BxDF should support, including evaluation,
//! sampling, etc. Named BxDF to generalize between BRDFs, BTDFs, BSSRDFs, etc.
class BxDF
{
public:

    enum BxDFType
    {
        BXDFTYPE_HAS_REFLECTANCE   = 1 << 0,
        BXDFTYPE_HAS_TRANSMISSION  = 1 << 1, // Not yet implemented
        BXDFTYPE_DIFFUSE           = 1 << 2, // Working
        BXDFTYPE_GLOSSY            = 1 << 3, // Glossy specular lobe
        BXDFTYPE_SPECULAR          = 1 << 4, // Perfect specular lobe (mirror)
        BXDFTYPE_ALL               = BXDFTYPE_HAS_REFLECTANCE |
                                     BXDFTYPE_HAS_TRANSMISSION | 
                                     BXDFTYPE_DIFFUSE | 
                                     BXDFTYPE_GLOSSY | 
                                     BXDFTYPE_SPECULAR
    };

    BxDF(BxDFType type)
        : m_BxDFType(type) {};

    //! Checks if the input flags matches the BxDF flags exactly
    //! @param t                The input type to check against
    //! @return                 True if the input flags matches the BxDF flags exactly;
    //!                         False otherwise.
    exrBool MatchesFlags(BxDFType t) const { return (m_BxDFType & t) == m_BxDFType; };

    //! Checks if the BxDF flags have at least all the flags in the input type
    //! @param t                The input type to check against
    //! @return                 True if the BxDF flags have at least the flags in the input;
    //!                         False otherwise.
    exrBool HasFlags(BxDFType t) const { return (m_BxDFType & t) == t; }

    //! @brief Computes the value of the distribution given a pair of directions
    //!
    //! Given two input directions, determine the color based on the BxDF. This is
    //! the f(p, wo, wi) term in the rendering equation. This function does not support
    //! delta BxDFs (i.e., mirror reflection) unless the exact wo is known.
    //! 
    //! @param wo               The outgoing direction of the BxDF
    //! @param wi               The incoming direction of the BxDF
    //! @return                 The output value of the BxDF
    virtual exrSpectrum f(const exrVector3& wo, const exrVector3& wi) const = 0;

    //! @brief Computes the direction of incident light given an outgoing direction
    //! 
    //! Given an outgoing direction, compute a possible incoming direction of incident light.
    //! The pdf of the associated incoming direction is also returned. This function allows
    //! the computation of wi for evaluating delta BxDFs. The output direction is chosen 
    //! according to a distribution that is similar to the BxDF's scattering function.
    //!
    //! @param wo               The outgoing direction of the BxDF
    //! @param wi               Outputs a possible incoming direction given wo
    //! @param pdf              Outputs the pdf associated with wi->wo
    //! @return                 The output value of the BxDF
    virtual exrSpectrum Sample_f(const exrVector3& wo, exrVector3* wi, exrFloat* pdf) const;

    //! @brief Computes the hemispherical-directional reflectance of a outgoing direction.
    //! 
    //! Computes the total reflection in a given direction due to constant illumination over
    //! the hemisphere, or equivalently, the total reflection over the hemisphere given
    //! light from a given direction. Some BxDF can compute this value in closed form but
    //! most should perform monte-carlo integration over the hemisphere.
    //!
    //! @param wo               The input direction
    //! @param numSamples       The number of samples should monte-carlo integration be used
    //! @return                 The output value of the BxDF
    virtual exrSpectrum rho(const exrVector3& wo, exrU32 numSamples) const = 0;

    //! @brief Computes the pdf given a pair of directions
    //! 
    //! @param wo               The outgoing direction of the BxDF
    //! @param wi               The incoming direction of the BxDF
    //! @return                 The pdf associated with wi->wo
    virtual exrFloat Pdf(const exrVector3& wo, const exrVector3& wi) const;

protected:

    //! @brief Checks if a pair of directions is in the same hemisphere
    //! 
    //! @param wo               The outgoing direction of the BxDF
    //! @param wi               The incoming direction of the BxDF
    //! @return                 True if wo and wi are in the same hemisphere; false otherwise
    inline exrBool IsSameHemisphere(const exrVector3& wo, const exrVector3& wi) const { return wo.z * wi.z > 0; };

private:
    BxDFType m_BxDFType;
};

exrEND_NAMESPACE