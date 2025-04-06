#pragma once

#include <archpp/rule/Rule.hpp>

#include <archpp/inputs/BaseInputSelector.hpp>
#include <archpp/filters/BaseFilter.hpp>
#include <archpp/outcomes/BaseOutcome.hpp>

namespace archpp
{
    class RuleImpl : public Rule
    {
    public:
        RuleImpl(inputs::InputSelectorPtr input,
                 filters::FilterPtr filter,
                 outcomes::OutcomePtr outcome,
                 std::string message = "")
            : m_Input(std::move(input))
            , m_Filter(std::move(filter))
            , m_Outcome(std::move(outcome))
            , m_Message(std::move(message))
        {
        }

        bool evaluate(const cl::ASTContext& context) override;

        const ViolationList& violations() const override;

        void reset() override;

    private:
        void RecordViolations(const cl::ASTContext& context, const archpp::DeclResult& decls);

        void RecordViolation(const cl::ASTContext& context,
                             const cl::Decl* decl,
                             const std::string& violationMsg);

    private:
        inputs::InputSelectorPtr m_Input;
        filters::FilterPtr m_Filter;
        outcomes::OutcomePtr m_Outcome;

        std::string m_Message;
        ViolationList m_Violations;
    };
}