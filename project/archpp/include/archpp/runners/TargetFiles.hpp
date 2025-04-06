#pragma once

#include <regex>
#include <filesystem>
#include <unordered_map>

namespace archpp::runners
{
    struct TargetFile
    {
        const char* FilePath;

        explicit TargetFile(const char* path);
    };

    struct TargetDirectory
    {
        const char* DirectoryPath;
        bool Recursive;

        explicit TargetDirectory(const char* path, bool recursive = true);
    };

    struct TargetPattern
    {
        const char* DirectoryPath;
        std::regex Pattern;
        bool Recursive;

        explicit TargetPattern(const char* path,
                               std::regex pattern = std::regex("*"),
                               bool recursive = true);

        static TargetPattern CppProject(const char* directory, bool recursive = true)
        {
            return TargetPattern(
                directory, std::regex(".*\\.(cpp|h|hpp|ixx|ipp|cxx|cc|hxx|mxx|mpp)$"), recursive);
        }

        static TargetPattern CProject(const char* directory, bool recursive = true)
        {
            return TargetPattern(directory, std::regex(".*\\.(c|h)$"), recursive);
        }
    };

    class TargetFiles
    {
    private:
        using string_container = std::vector<std::string>;

    public:
        void add(const TargetFile& file);
        void add(const TargetDirectory& directory);
        void add(const TargetPattern& pattern);

    public:
        const auto& args() const noexcept
        {
            return m_Argv;
        }

        auto& mut_args() noexcept
        {
            return m_Argv;
        }

    private:
        void add_string(const std::filesystem::path& path)
        {
            m_Argv.emplace_back(path.string());
        }

        void add_string(std::string&& str)
        {
            m_Argv.emplace_back(std::move(str));
        }

    private:
        string_container m_Argv;
    };
}