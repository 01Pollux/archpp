#pragma once

#include <archpp/core/core.hpp>

namespace archpp
{
    class OutputFormatter
    {
    public:
        virtual ~OutputFormatter() = default;
        virtual void output(const ViolationList& violations) = 0;
    };

    using OutputFormatterPtr = std::shared_ptr<OutputFormatter>;
}