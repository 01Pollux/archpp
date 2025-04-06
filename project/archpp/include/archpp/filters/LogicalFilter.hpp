#pragma once

#include "BaseFilter.hpp"

namespace archpp::filters
{
    enum class LogicalOperation : uint8_t
    {
        And,
        Or,
        Xor,

        Nand,
        Nor,
        Xnor,
    };

    template<LogicalOperation Op>
    class LogicalFilter : public BaseFilter
    {
    public:
        LogicalFilter(FilterPtr left, FilterPtr right)
            : m_Left(std::move(left))
            , m_Right(std::move(right))
        {
        }

        DeclResult apply(const cl::ASTContext& context, const DeclResult& decls) override;

    private:
        FilterPtr m_Left;
        FilterPtr m_Right;
    };

    class TrueFilter : public BaseFilter
    {
    public:
        DeclResult apply(const cl::ASTContext& context, const DeclResult& decls) override
        {
            return decls;
        }
    };

    class FalseFilter : public BaseFilter
    {
    public:
        DeclResult apply(const cl::ASTContext& context, const DeclResult& decls) override
        {
            return {};
        }
    };
}

namespace archpp::filters
{
    inline FilterPtr none()
    {
        return std::make_shared<FalseFilter>();
    }

    inline FilterPtr all()
    {
        return std::make_shared<TrueFilter>();
    }

    inline FilterPtr operator&&(FilterPtr left, FilterPtr right)
    {
        using filter_type = LogicalFilter<LogicalOperation::And>;
        return std::make_shared<filter_type>(std::move(left), std::move(right));
    }

    inline FilterPtr operator||(FilterPtr left, FilterPtr right)
    {
        using filter_type = LogicalFilter<LogicalOperation::Or>;
        return std::make_shared<filter_type>(std::move(left), std::move(right));
    }

    inline FilterPtr operator^(FilterPtr left, FilterPtr right)
    {
        using filter_type = LogicalFilter<LogicalOperation::Xor>;
        return std::make_shared<filter_type>(std::move(left), std::move(right));
    }

    inline FilterPtr nand_(FilterPtr left, FilterPtr right)
    {
        using filter_type = LogicalFilter<LogicalOperation::Nand>;
        return std::make_shared<filter_type>(std::move(left), std::move(right));
    }

    inline FilterPtr nor_(FilterPtr left, FilterPtr right)
    {
        using filter_type = LogicalFilter<LogicalOperation::Nor>;
        return std::make_shared<filter_type>(std::move(left), std::move(right));
    }

    inline FilterPtr xnor_(FilterPtr left, FilterPtr right)
    {
        using filter_type = LogicalFilter<LogicalOperation::Xnor>;
        return std::make_shared<filter_type>(std::move(left), std::move(right));
    }

    inline FilterPtr operator!(FilterPtr filter)
    {
        return nand_(std::move(filter), all());
    }

    inline FilterPtr operator~(FilterPtr filter)
    {
        return nand_(std::move(filter), all());
    }
}
