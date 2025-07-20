#include <archpp/test/CppProjectFixture.hpp>

ARCHPP_SUITE_BEGIN_CPP(FunctionNamingRules)

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

ARCHPP_RULE(MethodNamingRule)
{
    using namespace archpp;
    return RuleBuilder()
        .message("Public methods should be in camelCase")
        .input(inputs::all_methods())
        .filter(filters::all_public() && //
                filters::with(
                    [](const cl::Decl& decl)
                    {
                        auto methodDecl = dyn_cast<cl::CXXMethodDecl>(&decl);
                        if (!methodDecl)
                        {
                            return false;
                        }

                        auto name = methodDecl->getNameAsString();
                        if (name.empty() || name.starts_with("operator"))
                        {
                            return false; // Skip operators
                        }

                        // Check if it's camelCase (starts with lowercase,
                        // contains uppercase)
                        return !std::islower(name[0]);
                    }))
        .outcome(outcomes::not_ok());
}

ARCHPP_RULE(ConstMethodRule)
{
    using namespace archpp;
    return RuleBuilder()
        .message("Getter methods should be const")
        .input(inputs::all_methods())
        .filter(filters::with(
            [](const cl::Decl& decl)
            {
                auto methodDecl = dyn_cast<cl::CXXMethodDecl>(&decl);
                if (!methodDecl)
                {
                    return false;
                }

                auto name = methodDecl->getNameAsString();
                // Check if it's a getter (starts with "get" or "is")
                bool isGetter = name.starts_with("get") || name.starts_with("is");

                return isGetter && !methodDecl->isConst() &&
                       methodDecl->param_empty(); // No parameters
            }))
        .outcome(outcomes::not_ok());
}

ARCHPP_SUITE_END_CPP()
