#pragma once

#include <vector>
#include <set>
#include <ranges>
#include <archpp/core/core.hpp>

namespace archpp::inputs
{
    class BaseInputSelector
    {
    public:
        virtual ~BaseInputSelector() = default;

    public:
        virtual DeclResult select(const cl::ASTContext& context) = 0;
    };

    using InputSelectorPtr = std::shared_ptr<BaseInputSelector>;
}