#pragma once

#include "StreamOutputFormatter.hpp"
#include <iostream>

namespace archpp
{
    class ConsoleOutputFormatter : public OutputFormatter
    {
    public:
        void output(const ViolationList& violations) override
        {
            for (const auto& violation : violations)
            {
                std::cerr << violation.pretty() << '\n';
            }

            std::cerr << "Total violations: " << violations.size() << '\n';
        }
    };
}