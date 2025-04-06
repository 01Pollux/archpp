#pragma once

#include <archpp/inputs/BaseInputSelector.hpp>

namespace archpp::inputs
{
    class NoSystemHeadersInputSelector : public BaseInputSelector
    {
    public:
        NoSystemHeadersInputSelector(InputSelectorPtr input) noexcept
            : m_Input(std::move(input))
        {
        }

    public:
        DeclResult select(const cl::ASTContext& context) override
        {
            auto result = m_Input->select(context);

            std::erase_if(result,
                          [&sm = context.getSourceManager()](const auto& decl)
                          { return in_system_headers(sm, decl); });

            return result;
        }

    private:
        InputSelectorPtr m_Input;
    };

}

namespace archpp::inputs
{
    inline InputSelectorPtr no_system_headers(InputSelectorPtr input) noexcept
    {
        return std::make_shared<NoSystemHeadersInputSelector>(std::move(input));
    }
}