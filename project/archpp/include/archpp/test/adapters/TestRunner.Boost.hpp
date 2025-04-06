#pragma once

#define BOOST_AUTO_TEST_MAIN

#include <archpp/archpp.hpp>
#include <archpp/formatters/ConsoleOutputFormatter.hpp>
#include <boost/test/included/unit_test.hpp>

namespace archpp::test
{
    class TestRunnerFixture
    {
    public:
        TestRunnerFixture()
        {
            m_Runner.add<ConsoleOutputFormatter>();
        }

        void validate(Rule* rule)
        {
            BOOST_CHECK_MESSAGE(m_Runner.run_on(m_Files, rule),
                                "Rule violation detected, check the output for more details.");
        }

        void validate(const RulePtr& rule)
        {
            validate(rule.get());
        }

    protected:
        runners::Runner m_Runner;
        runners::TargetFiles m_Files;
    };
}

#define ARCHPP_RULE(RuleName)                               \
    namespace details                                       \
    {                                                       \
        archpp::RuleBuilder RuleName##_CreateRuleBuilder(); \
    }                                                       \
    BOOST_AUTO_TEST_CASE(RuleName##Test)                    \
    {                                                       \
        validate(details::RuleName##_CreateRuleBuilder());  \
    }                                                       \
    archpp::RuleBuilder details::RuleName##_CreateRuleBuilder()

#define ARCHPP_SUITE_BEGIN(SuiteName, Fixture) BOOST_FIXTURE_TEST_SUITE(SuiteName, Fixture)
#define ARCHPP_SUITE_END() BOOST_AUTO_TEST_SUITE_END()