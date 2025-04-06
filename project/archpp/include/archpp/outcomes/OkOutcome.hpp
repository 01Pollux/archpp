#pragma once

#include "BaseOutcome.hpp"

namespace archpp::outcomes
{
    class OkOutcome : public BaseOutcome
    {
    public:
        /// <summary>
        /// always returns true
        /// </summary>
        bool evaluate(const cl::ASTContext&, const DeclResult&) override
        {
            return true;
        }

        std::string message() const override
        {
            return "All elements pass the check";
        }
    };
}

namespace archpp::outcomes
{
    inline OutcomePtr ok()
    {
        return std::make_shared<OkOutcome>();
    }
}