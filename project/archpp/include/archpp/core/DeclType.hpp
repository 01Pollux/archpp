#pragma once

namespace archpp
{
    enum class DeclType : unsigned char
    {
        Class,
        Struct,
        Enum,
        Function,
        Method,
        Variable,
        Namespace,
        TypeAlias,
        TemplateParam,
        Any
    };
}

#define FOR_EACH_DECLTYPE(X)           \
    X(archpp::DeclType::Class)         \
    X(archpp::DeclType::Struct)        \
    X(archpp::DeclType::Enum)          \
    X(archpp::DeclType::Function)      \
    X(archpp::DeclType::Method)        \
    X(archpp::DeclType::Variable)      \
    X(archpp::DeclType::Namespace)     \
    X(archpp::DeclType::TypeAlias)     \
    X(archpp::DeclType::TemplateParam) \
    X(archpp::DeclType::Any)