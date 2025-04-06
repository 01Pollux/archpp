#pragma once

#include "TestRunner.hpp"

namespace archpp::test
{
    class CProjectFixture : public TestRunnerFixture
    {
    public:
        CProjectFixture()
        {
            m_Files.add(runners::TargetPattern::CProject(ARCHPP_PROJECT_DIR));
        }
    };
}

#define ARCHPP_SUITE_BEGIN_C(SuiteName) ARCHPP_SUITE_BEGIN(SuiteName, archpp::test::CProjectFixture)
#define ARCHPP_SUITE_END_C() ARCHPP_SUITE_END()
