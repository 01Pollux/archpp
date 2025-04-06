#pragma once

#include "OutputFormatter.hpp"
#include <fstream>

namespace archpp
{
    class FileOutputFormatter : public OutputFormatter
    {
    public:
        explicit FileOutputFormatter(std::string filename)
            : m_Filename(std::move(filename))
        {
        }

        void output(const ViolationList& violations) override
        {
            std::ofstream file(m_Filename);

            for (const auto& violation : violations)
            {
                file << violation.pretty() << '\n';
            }

            file << "Total violations: " << violations.size() << '\n';
        }

    private:
        std::string m_Filename;
    };
}
