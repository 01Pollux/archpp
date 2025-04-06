#pragma once

#include <vector>
#include <set>
#include <clang/AST/AST.h>

namespace archpp
{
    namespace cl = clang;

    using DeclResult = std::vector<const cl::Decl*>;

    [[nodiscard]]
    inline std::set<const cl::Decl*> as_set(const DeclResult& decls)
    {
        return {decls.begin(), decls.end()};
    }
}