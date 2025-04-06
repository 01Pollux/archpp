#pragma once

#include <archpp/inputs/BaseInputSelector.hpp>
#include <archpp/filters/BaseFilter.hpp>
#include <archpp/outcomes/BaseOutcome.hpp>
#include "Rule.hpp"

namespace archpp
{
    class RuleBuilder
    {
    public:
        RuleBuilder& input(inputs::InputSelectorPtr input)
        {
            m_Input = std::move(input);
            return *this;
        }

        RuleBuilder& filter(filters::FilterPtr filter)
        {
            m_Filter = std::move(filter);
            return *this;
        }

        RuleBuilder& outcome(outcomes::OutcomePtr outcome)
        {
            m_Outcome = std::move(outcome);
            return *this;
        }

        RuleBuilder& message(std::string&& message)
        {
            m_Message = std::move(message);
            return *this;
        }

    public:
        RulePtr build() const;

        operator RulePtr() const
        {
            return build();
        }

    private:
        inputs::InputSelectorPtr m_Input;
        filters::FilterPtr m_Filter;
        outcomes::OutcomePtr m_Outcome;

        std::string m_Message;
    };
}