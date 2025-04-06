#pragma once

#include "StreamOutputFormatter.hpp"
#include <iostream>

namespace archpp
{
    class OutputFormatterResultContainer : public OutputFormatter
    {
    public:
        void output(const ViolationList& violations) override
        {
            m_Violations = violations;
        }

        [[nodiscard]] const auto& violations() const
        {
            return m_Violations;
        }

    private:
        ViolationList m_Violations;
    };
}