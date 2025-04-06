#pragma once

#include "FileOutputFormatter.hpp"
#include <iostream>

namespace archpp
{
    class JsonOutputFormatter : public OutputFormatter
    {
    public:
        explicit JsonOutputFormatter(std::string filename)
            : m_Filename(std::move(filename))
        {
        }

        void output(const std::vector<Violation>& violations) override
        {
            std::ofstream file(m_Filename);
            if (!file)
            {
                std::cerr << "Failed to open file: " << m_Filename << std::endl;
                return;
            }

            file << "{\n  \"violations\": [\n";

            for (size_t i = 0; i < violations.size(); ++i)
            {
                const auto& v = violations[i];
                file << "    {\n"
                     << "      \"message\": \"" << escapeJsonString(v.message) << "\",\n"
                     << "      \"filename\": \"" << escapeJsonString(v.filename) << "\",\n"
                     << "      \"line\": " << v.line << ",\n"
                     << "      \"column\": " << v.column << "\n"
                     << "    }";

                if (i < violations.size() - 1)
                {
                    file << ",";
                }
                file << "\n";
            }

            file << "  ]\n}\n";
        }

    private:
        std::string escapeJsonString(const std::string& input)
        {
            std::string result;
            result.reserve(input.size() + 10); // Some extra space for escapes

            for (char c : input)
            {
                switch (c)
                {
                    case '\"':
                        result += "\\\"";
                        break;
                    case '\\':
                        result += "\\\\";
                        break;
                    case '\b':
                        result += "\\b";
                        break;
                    case '\f':
                        result += "\\f";
                        break;
                    case '\n':
                        result += "\\n";
                        break;
                    case '\r':
                        result += "\\r";
                        break;
                    case '\t':
                        result += "\\t";
                        break;
                    default:
                        if (static_cast<unsigned char>(c) < 0x20)
                        {
                            char buf[7];
                            snprintf(buf, sizeof(buf), "\\u%04x", c);
                            result += buf;
                        }
                        else
                        {
                            result += c;
                        }
                }
            }

            return result;
        }

    private:
        std::string m_Filename;
    };
}
