#include <archpp/rule/RuleBuilder.hpp>
#include "RuleImpl.hpp"

namespace archpp
{
    RulePtr RuleBuilder::build() const
    {
        return std::make_unique<RuleImpl>(m_Input, m_Filter, m_Outcome, m_Message);
    }
}