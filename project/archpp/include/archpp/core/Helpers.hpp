#pragma once

#include "TypeDefs.hpp"
#include <clang/Basic/SourceManager.h>

namespace archpp
{
    inline bool in_system_headers(const cl::SourceManager& sm, const cl::Decl* decl)
    {
        auto loc = decl->getLocation();
        return sm.isInSystemHeader(loc);
    }

    inline bool in_system_macros(const cl::SourceManager& sm, const cl::Decl* decl)
    {
        auto loc = decl->getLocation();
        return sm.isInSystemMacro(loc);
    }
}