#pragma once

#include "BaseFilter.hpp"

namespace archpp::filters
{
    class WithoutFilter : public BaseFilter
    {
    public:
        using predicate_type = std::function<bool(const cl::ASTContext&, const cl::Decl&)>;

        explicit WithoutFilter(predicate_type predicate)
            : m_Predicate(std::move(predicate))
        {
        }

        DeclResult apply(const cl::ASTContext& context, const DeclResult& decls) override
        {
            DeclResult result;
            for (auto decl : decls)
            {
                if (!m_Predicate(context, *decl))
                {
                    result.push_back(decl);
                }
            }
            return result;
        }

    private:
        predicate_type m_Predicate;
    };
}

namespace archpp::filters
{
    namespace wihout_details
    {
        // clang-format off
        // void(const cl::Decl& decl)
        template<typename T>
        concept predicate_without_context =
            std::invocable<T, const cl::Decl&> &&
            std::convertible_to<std::invoke_result_t<T, const cl::Decl&>, bool>;

        // void(const cl::ASTContext& context, const cl::Decl& decl)
        template<typename T>
        concept predicate_with_context =
            std::invocable<T, const cl::ASTContext&, const cl::Decl&> &&
            std::convertible_to<std::invoke_result_t<T, const cl::ASTContext&, const cl::Decl&>, bool>;
        // clang-format on
    }

    template<wihout_details::predicate_without_context Pred>
    inline FilterPtr without(Pred&& predicate)
    {
        auto pred = [predicate = std::forward<Pred>(predicate)](
                        const cl::ASTContext&, const cl::Decl& decl) { return predicate(decl); };
        return std::make_shared<WithoutFilter>(std::move(pred));
    }

    template<wihout_details::predicate_with_context Pred>
    inline FilterPtr without(Pred&& predicate)
    {
        return std::make_shared<WithoutFilter>(std::forward<Pred>(predicate));
    }
}