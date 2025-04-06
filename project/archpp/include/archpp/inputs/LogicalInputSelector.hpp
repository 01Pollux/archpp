#pragma once

#include <archpp/inputs/BaseInputSelector.hpp>

namespace archpp::inputs
{
    enum class LogicalOperation
    {
        Union,
        Intersection
    };

    template<LogicalOperation Op>
    class LogicalInputSelector : public BaseInputSelector
    {
    public:
        LogicalInputSelector(InputSelectorPtr left, InputSelectorPtr right) noexcept
            : m_Left(std::move(left))
            , m_Right(std::move(right))
        {
        }

    public:
        DeclResult select(const cl::ASTContext& context) override;

    private:
        InputSelectorPtr m_Left;
        InputSelectorPtr m_Right;
    };

}

namespace archpp::inputs
{
    // Operators for combining input selectors
    inline InputSelectorPtr operator||(InputSelectorPtr left, InputSelectorPtr right)
    {
        using selector_type = LogicalInputSelector<LogicalOperation::Union>;
        return std::make_shared<selector_type>(std::move(left), std::move(right));
    }

    inline InputSelectorPtr operator&&(InputSelectorPtr left, InputSelectorPtr right)
    {
        using selector_type = LogicalInputSelector<LogicalOperation::Intersection>;
        return std::make_shared<selector_type>(std::move(left), std::move(right));
    }
}