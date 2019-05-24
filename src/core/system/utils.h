#pragma once

// To detect multiple namespace opening which might be caused by using namespace elixir usage.
// if you get an error similar to the one below, it probably means that a using namespace elixir; statement
// has been found in an include file used by the source file.
// error C2872: 'inElixirNamespace' : ambiguous symbol
// To help you find the source, you should activate the display of file inclusion in advanced C++ settings of the project.

#ifdef EXR_USE_NAMESPACE
enum { InElixirNamespace = false };
namespace elixir { enum { InElixirNamespace = true }; }
#define exrBEGIN_NAMESPACE      exrStaticAssertMsg(!InElixirNamespace, "elixir namespace not previously closed"); namespace elixir {
#define exrEND_NAMESPACE        } exrStaticAssertMsg(!InElixirNamespace, "elixir namespace not previously closed");
#else
#define exrBEGIN_NAMESPACE      ((void)0)
#define exrEND_NAMESPACE        ((void)0)
#endif

#define exrTest                 {}