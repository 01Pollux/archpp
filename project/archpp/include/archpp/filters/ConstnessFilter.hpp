#pragma once

#include <iostream>
#include "WithFilter.hpp"

namespace archpp::filters
{
    enum class ConstFilter
    {
        None = 0,
        Const = 1,
        ConstExpr = 2,
        ConstEval = 4,
        ConstInit = 8,
        All = Const | ConstExpr | ConstEval | ConstInit
    };
    inline ConstFilter operator|(ConstFilter lhs, ConstFilter rhs)
    {
        return static_cast<ConstFilter>(static_cast<int>(lhs) | static_cast<int>(rhs));
    }
    inline ConstFilter operator&(ConstFilter lhs, ConstFilter rhs)
    {
        return static_cast<ConstFilter>(static_cast<int>(lhs) & static_cast<int>(rhs));
    }

    inline FilterPtr is_const(ConstFilter filter = ConstFilter::All)
    {
        return std::make_shared<WithFilter>(
            [filter](const cl::ASTContext&, const cl::Decl& decl)
            {
                if (auto varDecl = dyn_cast<cl::VarDecl>(&decl))
                {
                    if (varDecl->isConstexpr())
                    {
                        return (filter & ConstFilter::ConstExpr) != ConstFilter::None;
                    }
                    else if (varDecl->getType().isConstQualified())
                    {
                        return (filter & ConstFilter::Const) != ConstFilter::None;
                    }
                }
                else if (auto functionDecl = dyn_cast<cl::FunctionDecl>(&decl))
                {
                    switch (functionDecl->getConstexprKind())
                    {
                        case cl::ConstexprSpecKind::Constexpr:
                            return (filter & ConstFilter::ConstExpr) != ConstFilter::None;
                        case cl::ConstexprSpecKind::Consteval:
                            return (filter & ConstFilter::ConstEval) != ConstFilter::None;
                        case cl::ConstexprSpecKind::Constinit:
                            return (filter & ConstFilter::ConstInit) != ConstFilter::None;
                    }
                }
                return filter == ConstFilter::None;
            });
    }
} // namespace archpp