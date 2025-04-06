#pragma once

#include "RuleImpl.hpp"

#include <clang/Basic/SourceManager.h>

namespace archpp
{
    bool RuleImpl::evaluate(const cl::ASTContext& context)
    {
        auto decls = m_Input->select(context);

        auto filteredDecls = m_Filter->apply(context, decls);

        bool result = m_Outcome->evaluate(context, filteredDecls);

        if (!result)
        {
            RecordViolations(context, filteredDecls);
        }

        return result;
    }

    const ViolationList& RuleImpl::violations() const
    {
        return m_Violations;
    }

    void RuleImpl::reset()
    {
        m_Violations.clear();
    }

    void RuleImpl::RecordViolations(const cl::ASTContext& context, const archpp::DeclResult& decls)
    {
        auto violationMsg = m_Message;
        if (violationMsg.empty())
        {
            violationMsg = m_Outcome->message();
        }

        for (auto decl : decls)
        {
            RecordViolation(context, decl, violationMsg);
        }
    }

    void RuleImpl::RecordViolation(const cl::ASTContext& context,
                                   const cl::Decl* decl,
                                   const std::string& violationMsg)
    {
        auto& sm = context.getSourceManager();

        auto loc = decl->getLocation();
        if (loc.isValid())
        {
            auto fileEntry = sm.getFileEntryForID(sm.getFileID(loc));
            std::string filename = fileEntry ? sm.getFilename(loc).str() : "<unknown>";

            auto decomposedLoc = sm.getDecomposedLoc(loc);
            unsigned line = sm.getLineNumber(decomposedLoc.first, decomposedLoc.second);
            unsigned column = sm.getColumnNumber(decomposedLoc.first, decomposedLoc.second);

            std::string detailedMsg = violationMsg;
            if (auto namedDecl = dyn_cast<cl::NamedDecl>(decl))
            {
                detailedMsg += " [" + namedDecl->getNameAsString() + "]";
            }

            m_Violations.push_back({detailedMsg, filename, line, column});
        }
    }
}