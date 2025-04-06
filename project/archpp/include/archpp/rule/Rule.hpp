#pragma once

#include <vector>
#include <archpp/core/core.hpp>

namespace archpp
{
    class Rule
    {
    public:
        virtual ~Rule() = default;
        virtual bool evaluate(const cl::ASTContext& context) = 0;
        virtual const ViolationList& violations() const = 0;
        virtual void reset() = 0;
    };

    using RulePtr = std::unique_ptr<Rule>;
}