#include <archpp/filters/LogicalFilter.hpp>

namespace archpp::filters
{
    template<>
    auto LogicalFilter<LogicalOperation::And>::apply(const cl::ASTContext& context,
                                                     const std::vector<const cl::Decl*>& decls)
        -> DeclResult
    {
        auto leftResult = m_Left->apply(context, decls);
        return m_Right->apply(context, leftResult);
    }

    template<>
    auto LogicalFilter<LogicalOperation::Or>::apply(const cl::ASTContext& context,
                                                    const std::vector<const cl::Decl*>& decls)
        -> DeclResult
    {
        auto leftResult = m_Left->apply(context, decls);
        auto rightResult = m_Right->apply(context, decls);

        DeclResult result;
        std::set_union(leftResult.begin(),
                       leftResult.end(),
                       rightResult.begin(),
                       rightResult.end(),
                       std::back_inserter(result));
        return result;
    }

    template<>
    auto LogicalFilter<LogicalOperation::Xor>::apply(const cl::ASTContext& context,
                                                     const std::vector<const cl::Decl*>& decls)
        -> DeclResult
    {
        auto leftResult = m_Left->apply(context, decls);
        auto rightResult = m_Right->apply(context, decls);

        DeclResult result;
        std::set_symmetric_difference(leftResult.begin(),
                                      leftResult.end(),
                                      rightResult.begin(),
                                      rightResult.end(),
                                      std::back_inserter(result));
        return result;
    }

    template<>
    auto LogicalFilter<LogicalOperation::Nand>::apply(const cl::ASTContext& context,
                                                      const std::vector<const cl::Decl*>& decls)
        -> DeclResult
    {
        auto leftResult = m_Left->apply(context, decls);
        auto rightResult = m_Right->apply(context, decls);

        DeclResult result;
        std::set_difference(leftResult.begin(),
                            leftResult.end(),
                            rightResult.begin(),
                            rightResult.end(),
                            std::back_inserter(result));
        return result;
    }

    template<>
    auto LogicalFilter<LogicalOperation::Nor>::apply(const cl::ASTContext& context,
                                                     const std::vector<const cl::Decl*>& decls)
        -> DeclResult
    {
        auto leftResult = m_Left->apply(context, decls);
        auto rightResult = m_Right->apply(context, decls);

        DeclResult result;
        std::set_intersection(leftResult.begin(),
                              leftResult.end(),
                              rightResult.begin(),
                              rightResult.end(),
                              std::back_inserter(result));
        return result;
    }

    template<>
    auto LogicalFilter<LogicalOperation::Xnor>::apply(const cl::ASTContext& context,
                                                      const std::vector<const cl::Decl*>& decls)
        -> DeclResult
    {
        auto leftResult = m_Left->apply(context, decls);
        auto rightResult = m_Right->apply(context, decls);

        DeclResult result;
        std::set_difference(leftResult.begin(),
                            leftResult.end(),
                            rightResult.begin(),
                            rightResult.end(),
                            std::back_inserter(result));

        std::set_difference(
            decls.begin(), decls.end(), result.begin(), result.end(), std::back_inserter(result));
        return result;
    }
}