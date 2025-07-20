#include <archpp/test/CppProjectFixture.hpp>

ARCHPP_SUITE_BEGIN_CPP(VariableNamingRules)

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
        .message("Global variables should be avoided")
        .input(inputs::all_variables())
        .filter(filters::with(
            [](const cl::Decl& decl)
            {
                auto varDecl = dyn_cast<cl::VarDecl>(&decl);
                if (!varDecl)
                {
                    return false;
                }

                // Check if it's a global variable (not in a function or class)
                auto context = varDecl->getDeclContext();
                return context->isFileContext() && !varDecl->isStaticLocal();
            }))
        .outcome(outcomes::not_ok());
}

ARCHPP_RULE(ConstantNamingRule)
{
    using namespace archpp;
    return RuleBuilder()
        .message("Constants should be in UPPER_CASE")
        .input(inputs::all_variables())
        .filter(filters::is_const(filters::ConstFilter::All) //
                && filters::with(
                       [](const cl::Decl& decl)
                       {
                           auto namedDecl = cast<cl::NamedDecl>(&decl);
                           auto name = namedDecl->getName();
                           if (name.empty())
                           {
                               return false;
                           }

                           // Check if name is in UPPER_CASE
                           return !std::all_of(
                               name.begin(),
                               name.end(),
                               [](char c)
                               { return std::isupper(c) || c == '_' || std::isdigit(c); });
                       }))
        .outcome(outcomes::not_ok());
}

ARCHPP_RULE(MemberVariableNamingRule)
{
    using namespace archpp;
    return RuleBuilder()
        .message("Private member variables should start with 'm_' prefix")
        .input(inputs::all_variables())
        .filter(filters::all_private() && filters::with(
                                              [](const cl::Decl& decl)
                                              {
                                                  auto fieldDecl = dyn_cast<cl::FieldDecl>(&decl);
                                                  if (!fieldDecl)
                                                  {
                                                      return false;
                                                  }

                                                  auto name = fieldDecl->getName();
                                                  return !name.empty() && !name.starts_with("m_");
                                              }))
        .outcome(outcomes::not_ok());
}

ARCHPP_SUITE_END_CPP()
