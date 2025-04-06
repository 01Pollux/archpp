#pragma once

#include <regex>
#include <archpp/inputs/BaseInputSelector.hpp>

namespace archpp::inputs
{
    class FileInputSelector : public BaseInputSelector
    {
    public:
        using predicate_type = std::function<bool(const std::string&)>;

        explicit FileInputSelector(predicate_type&& predicate)
            : m_Predicate(std::move(predicate))
        {
        }

    public:
        DeclResult select(const cl::ASTContext& context) override;

    private:
        predicate_type m_Predicate;
    };
}

namespace archpp::inputs
{
    inline InputSelectorPtr all_headers()
    {
        using namespace std::string_view_literals;

        static constexpr std::string_view extensions[] = {".h"sv, ".hpp"sv, ".hxx"sv, ".hh"sv};
        auto predicate = [](const std::string& filename)
        {
            return std::ranges::any_of(
                extensions, [&filename](const auto& ext) { return filename.ends_with(ext); });
        };
        return std::make_shared<FileInputSelector>(std::move(predicate));
    }

    inline InputSelectorPtr all_sources()
    {
        using namespace std::string_view_literals;

        static constexpr std::string_view extensions[] = {".cpp"sv, ".cc"sv, ".cxx"sv, ".c"sv};
        auto predicate = [](const std::string& filename)
        {
            return std::ranges::any_of(
                extensions, [&filename](const auto& ext) { return filename.ends_with(ext); });
        };
        return std::make_shared<FileInputSelector>(std::move(predicate));
    }

    inline InputSelectorPtr all_files()
    {
        return std::make_shared<FileInputSelector>([](const std::string&) { return true; });
    }

    inline InputSelectorPtr files_matching(std::regex pattern)
    {
        auto predicate = [pattern = std::move(pattern)](const std::string& filename)
        { return std::regex_match(filename, pattern); };
        return std::make_shared<FileInputSelector>(std::move(predicate));
    }

    inline InputSelectorPtr files_matching(std::string_view pattern)
    {
        return files_matching(std::regex(pattern.begin(), pattern.end()));
    }
}