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

#include "tslmanager.h"
#include "core/bsdf/bxdf.h"
#include "core/bsdf/lambert.h"
#include "core/bsdf/microfacet.h"
#include "core/bsdf/orennayar.h"
#include "core/material/matte.h"

exrBEGIN_NAMESPACE

using namespace Tsl_Namespace;

IMPLEMENT_TSLGLOBAL_BEGIN(TslGlobal)
IMPLEMENT_TSLGLOBAL_VAR(Tsl_float3, albedo)
IMPLEMENT_TSLGLOBAL_VAR(Tsl_float3, specular)
IMPLEMENT_TSLGLOBAL_VAR(Tsl_float3, position)
IMPLEMENT_TSLGLOBAL_VAR(Tsl_float,  roughness)
IMPLEMENT_TSLGLOBAL_END()

// Although it is possible to have different tsl global registered for different material types, this sample only uses one.
static TslGlobal g_tsl_global;

// The closure ids
static ClosureID g_ClosureOrenNayar = INVALID_CLOSURE_ID;

// This is the call back function for handling things like compiling errors and texture loading stuff.
class ShadingSystemInterfaceSimple : public ShadingSystemInterface {
public:
    // Simply fetch some memory from the memory pool
    void* allocate(unsigned int size) const override {
        return EXR_STATIC_ALLOC_ARRAY(char, size);
    }

    void catch_debug(const TSL_DEBUG_LEVEL level, const char* error) const override {
        printf("%s\n", error);
    }

    // Sample texture 2d
    void    sample_2d(const void* texture, float u, float v, float3& color) const override {
        // not implemented
    }

    void    sample_alpha_2d(const void* texture, float u, float v, float& alpha) const override {
        // not implemented
    }
};

// Matte material uses the Oren Nayar BRDF
bool initialize_matte_material(Material* matte) {
    constexpr auto shader_source = R"(
        shader test_shader(out closure bxdf){
            color albedo = global_value<albedo>;
            color roughness = global_value<roughness>;
            vector position = global_value<position>;

            float u = position.x / 5.5f + 0.51;
            float z = position.y / 5.5f;

            color temp;
            temp.r = u;
            temp.g = z;
            temp.b = 0.5;

            bxdf = make_closure<orennayar>(temp, roughness);
        }
    )";

    // Get the instance of TSL system
    auto& shading_system = Tsl_Namespace::ShadingSystem::get_instance();

    // Make a new shading context, instead of making a new context, renders can also cache a few shading context at the beginning.
    // And reuse them any time they are needed as long as no two threads are accessing the same shading context at the same time.
    auto shading_context = shading_system.make_shading_context();
    if (!shading_context)
        return false;

    // Create the shader unit template
    matte->m_ShaderTemplate = shading_context->begin_shader_unit_template("orennayar");

    if (!matte->m_ShaderTemplate)
        return false;

    // Register the TSL global for this shader unit template.
    auto ret = matte->m_ShaderTemplate->register_tsl_global(g_tsl_global.m_var_list);
    if (!ret)
        return false;

    // Compile the shader source code.
    ret = matte->m_ShaderTemplate->compile_shader_source(shader_source);
    if (!ret)
        return false;

    // Indicating the end of the shader unit template creation process.
    auto resolved_ret = shading_context->end_shader_unit_template(matte->m_ShaderTemplate.get());
    if (resolved_ret != TSL_Resolving_Status::TSL_Resolving_Succeed)
        return false;

    // make a shader instance
    matte->m_ShaderInstance = matte->m_ShaderTemplate->make_shader_instance();
    if (!matte->m_ShaderInstance)
        return false;

    // Resolve the shader instance
    resolved_ret = matte->m_ShaderInstance->resolve_shader_instance();
    if (resolved_ret != TSL_Resolving_Status::TSL_Resolving_Succeed)
        return false;

    // get the raw function pointer
    matte->m_ShaderFunction = (shader_raw_func)matte->m_ShaderInstance->get_function();

    return true;
}

/*
 * It does several things during TSL initialization.
 *   - Register the call back interface so that the ray tracer can handle some call back events like bxdf allocation.
 *   - Register all closure types used in this program. This needs to happen before shader compliation.
 *   - Create all materials by compiling its shader and cache the raw function pointer to be used later.
 */
void initialize_tsl_system() {
    exrProfile("Initializing Tiny Shading Language");

    // get the instance of tsl shading system
    auto& shading_system = ShadingSystem::get_instance();

    // register the call back functions
    std::unique_ptr<ShadingSystemInterfaceSimple> ssis = std::make_unique< ShadingSystemInterfaceSimple>();
    shading_system.register_shadingsystem_interface(std::move(ssis));

    // register closures
    g_ClosureOrenNayar = ClosureTypeOrenNayar::RegisterClosure();
}

void initialize_tsl_material(Material* matte)
{
    initialize_matte_material(matte);
}

BxDF* GetBxDF(const Material* const material, const SurfaceInteraction& si, MemoryArena& arena)
{
    TslGlobal tslGlobal;

    exrVector3 albedo = material->GetAlbedo().ToRGB();
    tslGlobal.albedo = make_float3(albedo.r, albedo.g, albedo.b);

    exrVector3 specular = material->GetSpecular().ToRGB();
    tslGlobal.specular = make_float3(specular.r, specular.g, specular.b);

    tslGlobal.roughness = material->GetRoughness();

    exrPoint3 position = si.m_Point;
    tslGlobal.position = make_float3(position.x, position.y, position.z);

    ClosureTreeNodeBase* closure = nullptr;
    material->m_ShaderFunction(&closure, &tslGlobal);

    if (closure->m_id == g_ClosureOrenNayar)
    {
        const ClosureTypeOrenNayar* bxdfParams = (const ClosureTypeOrenNayar*)closure->m_params;
        exrSpectrum outputAlbedo = exrSpectrum::FromRGB(exrVector3(bxdfParams->albedo.x, bxdfParams->albedo.y, bxdfParams->albedo.z));
        exrFloat outputRoughness = bxdfParams->roughness;
        return EXR_ARENA_ALLOC(arena, OrenNayar)(outputAlbedo, outputRoughness);
    }

    return EXR_ARENA_ALLOC(arena, Lambert)(exrSpectrum::FromRGB(exrVector3(1.0f, 0.0f, 1.0f)));
}

exrEND_NAMESPACE
