#pragma once

#include "TestRunner.hpp"

namespace archpp::test
{
    class CppProjectFixture : public TestRunnerFixture
    {
    public:
        CppProjectFixture()
        {
            m_Files.add(runners::TargetPattern::CppProject(ARCHPP_PROJECT_DIR));
        }
    };
}

#define ARCHPP_SUITE_BEGIN_CPP(SuiteName) \
    ARCHPP_SUITE_BEGIN(SuiteName, archpp::test::CppProjectFixture)
#define ARCHPP_SUITE_END_CPP() ARCHPP_SUITE_END()
