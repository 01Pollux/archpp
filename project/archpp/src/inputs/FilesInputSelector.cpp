#include <archpp/inputs/LogicalInputSelector.hpp>

namespace archpp::inputs
{
    template<>
    std::vector<const cl::Decl*> LogicalInputSelector<LogicalOperation::Union>::select(
        const cl::ASTContext& context)
    {
        auto left = m_Left->select(context);
        auto right = m_Right->select(context);

        std::vector<const cl::Decl*> results;
        std::set_union(
            left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(results));
        return results;
    }

    template<>
    std::vector<const cl::Decl*> LogicalInputSelector<LogicalOperation::Intersection>::select(
        const cl::ASTContext& context)
    {
        auto left = m_Left->select(context);
        auto right = m_Right->select(context);

        std::vector<const cl::Decl*> results;
        std::set_intersection(
            left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(results));
        return results;
    }
}