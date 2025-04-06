#include <archpp/inputs/AllOfTypeInputSelector.hpp>

#include <clang/AST/RecursiveASTVisitor.h>

namespace archpp::inputs
{
    class DeclVisitor : public cl::RecursiveASTVisitor<DeclVisitor>
    {
    public:
        DeclVisitor(std::vector<const cl::Decl*>& result,
                    const cl::SourceManager& sm,
                    DeclType type,
                    bool ignoreSystemHeaders)
            : m_Resuls(result)
            , m_Sm(sm)
            , m_Type(type)
            , m_IgnoreSystemHeaders(ignoreSystemHeaders)
        {
        }

        bool shouldVisitDecl(const cl::Decl* decl)
        {
            if (!decl)
                return false;

            // Skip declarations from system headers if requested
            if (m_IgnoreSystemHeaders)
            {
                auto loc = decl->getLocation();
                if (loc.isValid() && m_Sm.isInSystemHeader(loc))
                {
                    return false;
                }
            }

            return true;
        }

        bool VisitCXXRecordDecl(cl::CXXRecordDecl* decl)
        {
            if (!shouldVisitDecl(decl) || !decl->isCompleteDefinition())
                return true;

            if ((m_Type == DeclType::Class && decl->isClass()) ||
                (m_Type == DeclType::Struct && decl->isStruct()) || (m_Type == DeclType::Any))
            {
                m_Resuls.push_back(decl);
            }
            return true;
        }

        bool VisitEnumDecl(cl::EnumDecl* decl)
        {
            if (!shouldVisitDecl(decl))
                return true;

            if (m_Type == DeclType::Enum || m_Type == DeclType::Any)
            {
                m_Resuls.push_back(decl);
            }
            return true;
        }

        bool VisitFunctionDecl(cl::FunctionDecl* decl)
        {
            if (!shouldVisitDecl(decl) || !decl->isThisDeclarationADefinition())
                return true;

            if ((m_Type == DeclType::Function && !isa<cl::CXXMethodDecl>(decl)) ||
                (m_Type == DeclType::Method && isa<cl::CXXMethodDecl>(decl)) ||
                (m_Type == DeclType::Any))
            {
                m_Resuls.push_back(decl);
            }
            return true;
        }

        bool VisitVarDecl(cl::VarDecl* decl)
        {
            if (!shouldVisitDecl(decl))
                return true;

            if (m_Type == DeclType::Variable || m_Type == DeclType::Any)
            {
                m_Resuls.push_back(decl);
            }
            return true;
        }

        bool VisitNamespaceDecl(cl::NamespaceDecl* decl)
        {
            if (!shouldVisitDecl(decl))
                return true;

            if (m_Type == DeclType::Namespace || m_Type == DeclType::Any)
            {
                m_Resuls.push_back(decl);
            }
            return true;
        }

        bool VisitTypedefNameDecl(cl::TypedefNameDecl* decl)
        {
            if (!shouldVisitDecl(decl))
                return true;

            if (m_Type == DeclType::TypeAlias || m_Type == DeclType::Any)
            {
                m_Resuls.push_back(decl);
            }
            return true;
        }

        bool VisitTemplateTypeParmDecl(cl::TemplateTypeParmDecl* decl)
        {
            if (!shouldVisitDecl(decl))
                return true;

            if (m_Type == DeclType::TemplateParam || m_Type == DeclType::Any)
            {
                m_Resuls.push_back(decl);
            }
            return true;
        }

    private:
        std::vector<const cl::Decl*>& m_Resuls;
        const cl::SourceManager& m_Sm;
        DeclType m_Type;
        bool m_IgnoreSystemHeaders;
    };

    std::vector<const cl::Decl*> AllOfTypeInputSelector::select(const cl::ASTContext& context)
    {
        std::vector<const cl::Decl*> result;

        DeclVisitor visitor(result, context.getSourceManager(), m_Type, m_IgnoreSystemHeaders);
        visitor.TraverseDecl(context.getTranslationUnitDecl());

        return result;
    }
}