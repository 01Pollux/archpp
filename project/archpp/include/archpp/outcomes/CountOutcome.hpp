#pragma once

#include "BaseOutcome.hpp"

namespace archpp::outcomes
{
    enum class CountOutcomeComparisonOp
    {
        Equal,
        NotEqual,
        Greater,
        Less,
        GreaterEqual,
        LessEqual
    };

    template<CountOutcomeComparisonOp Op>
    class CountOutcome : public BaseOutcome
    {
    public:
        CountOutcome(size_t count)
            : m_Count(count)
        {
        }

        bool evaluate(const cl::ASTContext&, const DeclResult& decls) override
        {
            switch (Op)
            {
                case CountOutcomeComparisonOp::Equal:
                    return decls.size() == m_Count;
                case CountOutcomeComparisonOp::NotEqual:
                    return decls.size() != m_Count;
                case CountOutcomeComparisonOp::Greater:
                    return decls.size() > m_Count;
                case CountOutcomeComparisonOp::Less:
                    return decls.size() < m_Count;
                case CountOutcomeComparisonOp::GreaterEqual:
                    return decls.size() >= m_Count;
                case CountOutcomeComparisonOp::LessEqual:
                    return decls.size() <= m_Count;
                default:
                    return false;
            }
        }

        std::string message() const override
        {
            const char* opStr = "";
            switch (Op)
            {
                case CountOutcomeComparisonOp::Equal:
                    opStr = "==";
                    break;
                case CountOutcomeComparisonOp::NotEqual:
                    opStr = "!=";
                    break;
                case CountOutcomeComparisonOp::Greater:
                    opStr = ">";
                    break;
                case CountOutcomeComparisonOp::Less:
                    opStr = "<";
                    break;
                case CountOutcomeComparisonOp::GreaterEqual:
                    opStr = ">=";
                    break;
                case CountOutcomeComparisonOp::LessEqual:
                    opStr = "<=";
                    break;
            }

            return std::format("Expected count {} {}", opStr, m_Count);
        }

    private:
        size_t m_Count;
    };
}

namespace archpp::outcomes
{
    inline OutcomePtr eq_(size_t count)
    {
        return std::make_shared<CountOutcome<CountOutcomeComparisonOp::Equal>>(count);
    }

    inline OutcomePtr ne_(size_t count)
    {
        return std::make_shared<CountOutcome<CountOutcomeComparisonOp::NotEqual>>(count);
    }

    inline OutcomePtr gt_(size_t count)
    {
        return std::make_shared<CountOutcome<CountOutcomeComparisonOp::Greater>>(count);
    }

    inline OutcomePtr lt_(size_t count)
    {
        return std::make_shared<CountOutcome<CountOutcomeComparisonOp::Less>>(count);
    }

    inline OutcomePtr ge_(size_t count)
    {
        return std::make_shared<CountOutcome<CountOutcomeComparisonOp::GreaterEqual>>(count);
    }

    inline OutcomePtr le_(size_t count)
    {
        return std::make_shared<CountOutcome<CountOutcomeComparisonOp::LessEqual>>(count);
    }
}