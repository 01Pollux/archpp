#include <archpp/test/CppProjectFixture.hpp>

ARCHPP_SUITE_BEGIN_CPP(ArchitecturalRules)

ARCHPP_RULE(NoPublicDataMembersRule)
{
    using namespace archpp;
    return RuleBuilder()
        .message("Classes should not have public data members")
        .input(inputs::all_classes())
        .filter(filters::with(
            [](const cl::Decl& decl)
            {
                auto cxxRecordDecl = dyn_cast<cl::CXXRecordDecl>(&decl);
                if (!cxxRecordDecl)
                {
                    return false;
                }

                // Check if the class has any public data members
                for (auto field : cxxRecordDecl->fields())
                {
                    if (field->getAccess() == cl::AS_public)
                    {
                        // Add diagnostic output
                        std::cout << "Found public field: " << field->getNameAsString()
                                  << " in class " << cxxRecordDecl->getNameAsString() << "\n";
                        return true; // Found a public data member
                    }
                }
                return false;
            }))
        .outcome(outcomes::not_ok());
}

ARCHPP_RULE(VirtualDestructorRule)
{
    using namespace archpp;
    return RuleBuilder()
        .message("Classes with virtual methods should have virtual destructors")
        .input(inputs::all_classes())
        .filter(filters::with(
            [](const cl::Decl& decl)
            {
                auto cxxRecordDecl = dyn_cast<cl::CXXRecordDecl>(&decl);
                if (!cxxRecordDecl || !cxxRecordDecl->isPolymorphic())
                {
                    return false;
                }

                // keep checking current class and parent
                for (const cl::DeclContext* current = cxxRecordDecl; current;
                     current = current->getParent())
                {
                    if (auto cxxRecord = dyn_cast<cl::CXXRecordDecl>(current))
                    {
                        if (auto destructor = cxxRecord->getDestructor();
                            destructor && destructor->isVirtual())
                        {
                            return false;
                        }
                    }
                }

                return true; // No destructor but has virtual methods
            }))
        .outcome(outcomes::not_ok());
}

ARCHPP_RULE(SingleResponsibilityRule)
{
    using namespace archpp;
    return RuleBuilder()
        .message("Classes should not have too many public methods (max 10)")
        .input(inputs::all_classes())
        .filter(filters::with(
            [](const cl::Decl& decl)
            {
                auto cxxRecordDecl = dyn_cast<cl::CXXRecordDecl>(&decl);
                if (!cxxRecordDecl)
                {
                    return false;
                }

                int publicMethodCount = 0;
                for (auto method : cxxRecordDecl->methods())
                {
                    if (method->getAccess() == cl::AS_public &&
                        !isa<cl::CXXConstructorDecl>(method) && !isa<cl::CXXDestructorDecl>(method))
                    {
                        publicMethodCount++;
                    }
                }

                return publicMethodCount > 10;
            }))
        .outcome(outcomes::not_ok());
}

ARCHPP_SUITE_END_CPP()
