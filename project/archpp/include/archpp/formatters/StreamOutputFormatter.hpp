#pragma once

#include "OutputFormatter.hpp"
#include <ostream>

namespace archpp
{
    class StreamOutputFormatter : public OutputFormatter
    {
    public:
        explicit StreamOutputFormatter(std::ostream& stream)
            : m_Stream(stream)
        {
        }

        void output(const ViolationList& violations) override
        {
            for (const auto& violation : violations)
            {
                m_Stream.write(std::bit_cast<const char*>(&violation), violation.pretty().size());
            }
        }

    private:
        std::ostream& m_Stream;
    };
}