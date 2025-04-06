#pragma once

#include "BaseOutcome.hpp"

namespace archpp::outcomes
{
    class NotOkOutcome : public BaseOutcome
    {
    public:
        /// <summary>
        /// returns false if there are any elements in the decls vector
        /// </summary>
        bool evaluate(const cl::ASTContext&, const DeclResult& decls) override
        {
            return decls.empty();
        }

        std::string message() const override
        {
            return "Elements found that don't meet the criteria";
        }
    };
}

namespace archpp::outcomes
{
    inline OutcomePtr not_ok()
    {
        return std::make_shared<NotOkOutcome>();
    }
}