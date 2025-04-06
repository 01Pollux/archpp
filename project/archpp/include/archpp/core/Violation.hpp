#pragma once

#include <string>
#include <format>
#include <vector>

namespace archpp
{
    struct Violation
    {
        std::string message;
        std::string filename;
        unsigned line;
        unsigned column;

        std::string pretty() const
        {
            return std::format("{}:{}:{}: {}", filename, line, column, message);
        }
    };

    using ViolationList = std::vector<Violation>;
}