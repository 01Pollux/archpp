#pragma once

#include "BaseFilter.hpp"

namespace archpp::filters
{
    enum class MemberVisibilityType : uint8_t
    {
        None = 0,
        Private = 1 << 0,
        Protected = 1 << 1,
        Public = 1 << 2
    };
    ARCHPP_ENUM_FLAGS_OR(MemberVisibilityType);
    ARCHPP_ENUM_FLAGS_AND(MemberVisibilityType);

    class MemberVisibility : public BaseFilter
    {
    public:
        explicit MemberVisibility(MemberVisibilityType flags) noexcept
            : m_VisibilityFlags(flags)
        {
        }

        DeclResult apply(const cl::ASTContext& context, const DeclResult& decls) override
        {
            DeclResult result;
            for (auto decl : decls)
            {
                auto namedDecl = cast<cl::NamedDecl>(decl);
                if (!namedDecl)
                {
                    continue;
                }

                auto access = namedDecl->getAccess();
                if (access == cl::AS_private &&
                    (m_VisibilityFlags & MemberVisibilityType::Public) !=
                        MemberVisibilityType::None)
                {
                    result.push_back(decl);
                }
                else if (access == cl::AS_protected &&
                         (m_VisibilityFlags & MemberVisibilityType::Protected) !=
                             MemberVisibilityType::None)
                {
                    result.push_back(decl);
                }
                else if (access == cl::AS_public &&
                         (m_VisibilityFlags & MemberVisibilityType::Private) !=
                             MemberVisibilityType::None)
                {
                    result.push_back(decl);
                }
            }
            return result;
        }

    private:
        MemberVisibilityType m_VisibilityFlags;
    };
}

namespace archpp::filters
{
    inline FilterPtr all_private()
    {
        return std::make_shared<MemberVisibility>(MemberVisibilityType::Private);
    }

    inline FilterPtr all_protected()
    {
        return std::make_shared<MemberVisibility>(MemberVisibilityType::Protected);
    }

    inline FilterPtr all_public()
    {
        return std::make_shared<MemberVisibility>(MemberVisibilityType::Public);
    }

    inline FilterPtr with_visiblity(MemberVisibilityType flags)
    {
        return std::make_shared<MemberVisibility>(flags);
    }
}