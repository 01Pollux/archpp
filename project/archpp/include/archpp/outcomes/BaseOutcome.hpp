#pragma once

#include <vector>
#include <string>
#include <archpp/core/core.hpp>

namespace archpp::outcomes
{
    class BaseOutcome
    {
    public:
        virtual ~BaseOutcome() = default;
        virtual bool evaluate(const cl::ASTContext&, const DeclResult& decls) = 0;
        virtual std::string message() const = 0;
    };

    using OutcomePtr = std::shared_ptr<BaseOutcome>;
}