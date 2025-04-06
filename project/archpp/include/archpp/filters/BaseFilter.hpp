#pragma once

#include <vector>
#include <archpp/core/core.hpp>

namespace archpp::filters
{
    class BaseFilter
    {
    public:
        virtual ~BaseFilter() = default;

    public:
        virtual DeclResult apply(const cl::ASTContext& context, const DeclResult& decls) = 0;
    };

    using FilterPtr = std::shared_ptr<BaseFilter>;
}
