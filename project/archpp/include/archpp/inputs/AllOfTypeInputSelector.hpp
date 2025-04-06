#pragma once

#include <archpp/inputs/BaseInputSelector.hpp>

namespace archpp::inputs
{
    class AllOfTypeInputSelector : public BaseInputSelector
    {
    public:
        AllOfTypeInputSelector(DeclType type, bool ignoreSystemHeaders)
            : m_Type(type)
            , m_IgnoreSystemHeaders(ignoreSystemHeaders)
        {
        }

        DeclResult select(const cl::ASTContext& context) override;

    private:
        DeclType m_Type;
        bool m_IgnoreSystemHeaders;
    };
}

namespace archpp::inputs
{
    inline InputSelectorPtr all_classes(bool ignoreSystemHeaders = true)
    {
        return std::make_shared<AllOfTypeInputSelector>(DeclType::Class, ignoreSystemHeaders);
    }

    inline InputSelectorPtr all_structs(bool ignoreSystemHeaders = true)
    {
        return std::make_shared<AllOfTypeInputSelector>(DeclType::Struct, ignoreSystemHeaders);
    }

    inline InputSelectorPtr all_enums(bool ignoreSystemHeaders = true)
    {
        return std::make_shared<AllOfTypeInputSelector>(DeclType::Enum, ignoreSystemHeaders);
    }

    inline InputSelectorPtr all_functions(bool ignoreSystemHeaders = true)
    {
        return std::make_shared<AllOfTypeInputSelector>(DeclType::Function, ignoreSystemHeaders);
    }

    inline InputSelectorPtr all_methods(bool ignoreSystemHeaders = true)
    {
        return std::make_shared<AllOfTypeInputSelector>(DeclType::Method, ignoreSystemHeaders);
    }

    inline InputSelectorPtr all_variables(bool ignoreSystemHeaders = true)
    {
        return std::make_shared<AllOfTypeInputSelector>(DeclType::Variable, ignoreSystemHeaders);
    }

    inline InputSelectorPtr all_namespaces(bool ignoreSystemHeaders = true)
    {
        return std::make_shared<AllOfTypeInputSelector>(DeclType::Namespace, ignoreSystemHeaders);
    }

    inline InputSelectorPtr all_type_aliases(bool ignoreSystemHeaders = true)
    {
        return std::make_shared<AllOfTypeInputSelector>(DeclType::TypeAlias, ignoreSystemHeaders);
    }

    inline InputSelectorPtr all_template_params(bool ignoreSystemHeaders = true)
    {
        return std::make_shared<AllOfTypeInputSelector>(DeclType::TemplateParam,
                                                        ignoreSystemHeaders);
    }

    inline InputSelectorPtr all_declarations(bool ignoreSystemHeaders = true)
    {
        return std::make_shared<AllOfTypeInputSelector>(DeclType::Any, ignoreSystemHeaders);
    }
}