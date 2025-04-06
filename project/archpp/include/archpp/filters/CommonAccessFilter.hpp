#pragma once

#include <regex>
#include "WithFilter.hpp"

namespace archpp::filters
{
    inline FilterPtr with_access(cl::AccessSpecifier access)
    {
        return with(
            [access](const cl::Decl& decl)
            {
                if (auto namedDecl = dyn_cast<cl::NamedDecl>(&decl))
                {
                    return namedDecl->getAccess() == access;
                }
                return false;
            });
    }

    inline FilterPtr is_public()
    {
        return with_access(cl::AS_public);
    }

    inline FilterPtr is_protected()
    {
        return with_access(cl::AS_protected);
    }

    inline FilterPtr is_private()
    {
        return with_access(cl::AS_private);
    }

    inline FilterPtr has_name_starting_with(std::string prefix)
    {
        return with(
            [prefix](const cl::Decl& decl)
            {
                if (auto namedDecl = dyn_cast<cl::NamedDecl>(&decl))
                {
                    auto name = namedDecl->getName();
                    return !name.empty() && name.starts_with(prefix);
                }
                return false;
            });
    }

    inline FilterPtr has_name_ending_with(std::string suffix)
    {
        return with(
            [suffix](const cl::Decl& decl)
            {
                if (auto namedDecl = dyn_cast<cl::NamedDecl>(&decl))
                {
                    auto name = namedDecl->getName();
                    return !name.empty() && name.ends_with(suffix);
                }
                return false;
            });
    }

    inline FilterPtr matches_regex(std::regex pattern)
    {
        return with(
            [pattern = std::move(pattern)](const cl::Decl& decl)
            {
                if (auto namedDecl = dyn_cast<cl::NamedDecl>(&decl))
                {
                    auto name = namedDecl->getName();
                    return !name.empty() && std::regex_match(name.begin(), name.end(), pattern);
                }
                return false;
            });
    }

    inline FilterPtr matches_regex(std::string_view pattern)
    {
        return matches_regex(std::regex(pattern.data(), pattern.size()));
    }
}
