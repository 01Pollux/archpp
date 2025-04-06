#include <archpp/runners/TargetFiles.hpp>

namespace archpp::runners
{
    namespace details
    {
        static void check_if_path_exists(const std::filesystem::path& path)
        {
            if (!std::filesystem::exists(path))
            {
                throw std::runtime_error(std::format("Path does not exist: {}", path.string()));
            }
        }

        static void check_if_regular_file(const std::filesystem::path& path)
        {
            if (!std::filesystem::is_regular_file(path))
            {
                throw std::runtime_error(std::format("Not a regular file: {}", path.string()));
            }
        }

        static void check_if_directory(const std::filesystem::path& path)
        {
            if (!std::filesystem::is_directory(path))
            {
                throw std::runtime_error(std::format("Not a directory: {}", path.string()));
            }
        }
    }

    //

    TargetFile::TargetFile(const char* path)
        : FilePath(path)
    {
        std::filesystem::path fs_path(path);
        details::check_if_path_exists(fs_path);
        details::check_if_regular_file(fs_path);
    }

    TargetDirectory::TargetDirectory(const char* path, bool recursive)
        : DirectoryPath(path)
        , Recursive(recursive)
    {
        std::filesystem::path fs_path(path);
        details::check_if_path_exists(fs_path);
        details::check_if_directory(fs_path);
    }

    TargetPattern::TargetPattern(const char* path,
                                 std::regex pattern,
                                 bool recursive)
        : DirectoryPath(path)
        , Pattern(std::move(pattern))
        , Recursive(recursive)
    {
        std::filesystem::path fs_path(path);
        details::check_if_path_exists(fs_path);
        details::check_if_directory(fs_path);
    }

    //

    void TargetFiles::add(const TargetFile& file)
    {
        m_Argv.emplace_back(file.FilePath);
    }

    void TargetFiles::add(const TargetDirectory& directory)
    {
        if (directory.Recursive)
        {
            for (const auto& entry :
                 std::filesystem::recursive_directory_iterator(directory.DirectoryPath))
            {
                if (entry.is_regular_file())
                {
                    add_string(entry.path());
                }
            }
        }
        else
        {
            for (const auto& entry : std::filesystem::directory_iterator(directory.DirectoryPath))
            {
                if (entry.is_regular_file())
                {
                    add_string(entry.path());
                }
            }
        }
    }

    void TargetFiles::add(const TargetPattern& pattern)
    {
        if (pattern.Recursive)
        {
            for (const auto& entry :
                 std::filesystem::recursive_directory_iterator(pattern.DirectoryPath))
            {
                if (entry.is_regular_file() &&
                    std::regex_match(entry.path().string(), pattern.Pattern))
                {
                    add_string(entry.path());
                }
            }
        }
        else
        {
            for (const auto& entry : std::filesystem::directory_iterator(pattern.DirectoryPath))
            {
                if (entry.is_regular_file() &&
                    std::regex_match(entry.path().string(), pattern.Pattern))
                {
                    add_string(entry.path());
                }
            }
        }
    }
}