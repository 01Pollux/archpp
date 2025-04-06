#include <archpp/test/CppProjectFixture.hpp>

ARCHPP_SUITE_BEGIN_CPP(NamingConventionRules)

ARCHPP_RULE(TypeNameCaseRule)
{
    using namespace archpp;
    return RuleBuilder()
        .message("Typenames should start with an uppercase letter")
        .input(inputs::all_classes() || inputs::all_structs())
        .filter(filters::with(
            [](const cl::Decl& decl)
            {
                auto namedDecl = cast<cl::NamedDecl>(&decl);
                auto name = namedDecl->getName();
                return !name.empty() && !isupper(name[0]);
            }))
        .outcome(outcomes::not_ok());
}

ARCHPP_RULE(FunctionCaseRule)
{
    using namespace archpp;
    return RuleBuilder()
        .message("Function names should all be in lowercase letters")
        .input(inputs::all_functions())
        .filter(filters::with(
            [](const cl::Decl& decl)
            {
                auto functionDecl = dyn_cast<cl::FunctionDecl>(&decl);
                if (!functionDecl)
                {
                    return false;
                }

                auto name = functionDecl->getNameAsString();
                return !name.empty() &&
                       !std::all_of(name.begin(),
                                    name.end(),
                                    [](char c) { return std::islower(c) || !std::isalpha(c); });
            }))
        .outcome(outcomes::not_ok());
}

ARCHPP_RULE(VariableCaseRule)
{
    using namespace archpp;
    return RuleBuilder()
        .message("Variable names should start with a lowercase letter")
        .input(inputs::all_variables())
        .filter(filters::is_const(filters::ConstFilter::None) //
                && filters::with(
                       [](const cl::Decl& decl)
                       {
                           auto namedDecl = cast<cl::NamedDecl>(&decl);
                           auto name = namedDecl->getName();
                           return !name.empty() && !islower(name[0]);
                       }))
        .outcome(outcomes::not_ok());
}

ARCHPP_RULE(GlobalVariableRule)
{
    using namespace archpp;
    return RuleBuilder()
        .message("Global variables are not allowed")
        .input(inputs::all_variables())
        .filter(filters::with(
            [](const cl::Decl& decl)
            {
                auto namedDecl = cast<cl::NamedDecl>(&decl);
                auto name = namedDecl->getName();
                return !name.empty() && name[0] == '_';
            }))
        .outcome(outcomes::not_ok());
}

ARCHPP_RULE(UnderscoreFunctionNameRule)
{
    using namespace archpp;
    return RuleBuilder()
        .message("Function names should not start with an underscore")
        .input(inputs::all_functions())
        .filter(filters::with(
            [](const cl::Decl& decl)
            {
                auto functionDecl = dyn_cast<cl::FunctionDecl>(&decl);
                if (!functionDecl)
                {
                    return false;
                }

                auto name = functionDecl->getNameAsString();
                return !name.empty() && name[0] == '_';
            }))
        .outcome(outcomes::not_ok());
}

ARCHPP_SUITE_END_CPP()