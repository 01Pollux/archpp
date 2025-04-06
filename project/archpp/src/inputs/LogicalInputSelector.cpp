#include <archpp/inputs/FilesInputSelector.hpp>

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>

namespace archpp::inputs
{
    class FileDeclVisitor : public cl::RecursiveASTVisitor<FileDeclVisitor>
    {
    public:
        FileDeclVisitor(DeclResult& result,
                        const FileInputSelector::predicate_type& predicate,
                        const cl::SourceManager& sm)
            : m_Result(result)
            , m_Predicate(std::move(predicate))
            , m_Sm(sm)
        {
        }

        bool VisitDecl(cl::Decl* decl)
        {
            if (auto loc = decl->getLocation(); loc.isValid())
            {
                auto filename = m_Sm.getFilename(loc);
                if (!filename.empty() && m_Predicate(filename.str()))
                {
                    m_Result.push_back(decl);
                }
            }
            return true;
        }

    public:
        DeclResult& m_Result;
        const FileInputSelector::predicate_type& m_Predicate;
        const cl::SourceManager& m_Sm;
    };

    DeclResult FileInputSelector::select(const cl::ASTContext& context)
    {
        DeclResult result;

        FileDeclVisitor visitor{result, m_Predicate, context.getSourceManager()};
        visitor.TraverseDecl(context.getTranslationUnitDecl());

        return result;
    }
}