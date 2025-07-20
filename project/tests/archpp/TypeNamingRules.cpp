#include <archpp/test/CppProjectFixture.hpp>

ARCHPP_SUITE_BEGIN_CPP(TypeNamingRules)

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

ARCHPP_RULE(ExceptionNamingRule)
{
    using namespace archpp;
    return RuleBuilder()
        .message("Exception classes should end with 'Exception'")
        .input(inputs::all_classes())
        .filter(filters::with(
            [](const cl::Decl& decl)
            {
                auto cxxRecordDecl = dyn_cast<cl::CXXRecordDecl>(&decl);
                if (!cxxRecordDecl)
                {
                    return false;
                }

                // Check if it inherits from std::exception or similar
                bool isExceptionClass = false;
                for (const auto& base : cxxRecordDecl->bases())
                {
                    if (auto baseType = base.getType()->getAsCXXRecordDecl())
                    {
                        auto baseName = baseType->getQualifiedNameAsString();
                        if (baseName.find("exception") != std::string::npos ||
                            baseName.find("Exception") != std::string::npos)
                        {
                            isExceptionClass = true;
                            break;
                        }
                    }
                }

                if (!isExceptionClass)
                {
                    return false;
                }

                auto name = cxxRecordDecl->getName();
                return !name.empty() && !name.ends_with("Exception");
            }))
        .outcome(outcomes::not_ok());
}

ARCHPP_RULE(EnumNamingRule)
{
    using namespace archpp;
    return RuleBuilder()
        .message("Enum names should start with an uppercase letter")
        .input(inputs::all_enums())
        .filter(filters::with(
            [](const cl::Decl& decl)
            {
                auto enumDecl = dyn_cast<cl::EnumDecl>(&decl);
                if (!enumDecl)
                {
                    return false;
                }

                auto name = enumDecl->getName();
                return !name.empty() && !isupper(name[0]);
            }))
        .outcome(outcomes::not_ok());
}

ARCHPP_SUITE_END_CPP()
