#include <archpp/runners/Runner.hpp>

#include <clang/Tooling/Tooling.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Frontend/CompilerInstance.h>

#include <iostream>

namespace clt = clang::tooling;

namespace archpp::runners
{
    class RuleASTConsumer : public cl::ASTConsumer
    {
    public:
        explicit RuleASTConsumer(Rule* rule)
            : m_Rule(rule)
        {
        }

        void HandleTranslationUnit(cl::ASTContext& context) override
        {
            m_Rule->evaluate(context);
        }

    private:
        Rule* m_Rule;
    };

    class RuleFrontendAction : public cl::ASTFrontendAction
    {
    public:
        explicit RuleFrontendAction(Rule* rule)
            : m_Rule(rule)
        {
        }

        std::unique_ptr<cl::ASTConsumer> CreateASTConsumer(cl::CompilerInstance& ci,
                                                           cl::StringRef) override
        {
            ci.getDiagnostics().setClient(&m_IgnoringDiagConsumer, false);
            return std::make_unique<RuleASTConsumer>(m_Rule);
        }

    private:
        Rule* m_Rule;

        cl::IgnoringDiagConsumer m_IgnoringDiagConsumer;
    };

    class RuleFrontendActionFactory : public clt::FrontendActionFactory
    {
    public:
        explicit RuleFrontendActionFactory(Rule* rule)
            : m_Rule(rule)
        {
        }

        std::unique_ptr<cl::FrontendAction> create() override
        {
            return std::make_unique<RuleFrontendAction>(m_Rule);
        }

    private:
        Rule* m_Rule;
    };

    Runner::Runner()
    {
    }

    bool Runner::run_code(const std::string& code, Rule* rule)
    {
        // Reset rules before running
        rule->reset();

        auto action = std::make_unique<RuleFrontendAction>(rule);

        // Run the tool on the code
        clt::runToolOnCode(std::move(action), code);

        // Collect all violations
        auto& violations = rule->violations();

        // Output violations using all formatters
        for (const auto& formatter : m_Formatters)
        {
            formatter->output(violations);
        }

        return violations.empty();
    }

    bool Runner::run_on(TargetFiles& targetFiles, Rule* rule)
    {
        // Reset rules before running
        rule->reset();

        clt::FixedCompilationDatabase compilationDatabase(".", {});
        clt::ClangTool tool(compilationDatabase, targetFiles.args());

        cl::IgnoringDiagConsumer diagConsumer;
        tool.setPrintErrorMessage(false);
        tool.setDiagnosticConsumer(&diagConsumer);

        // Run the tool
        RuleFrontendActionFactory actionFactory(rule);
        tool.run(&actionFactory);

        // Collect all violations
        auto& violations = rule->violations();

        // Output violations using all formatters
        for (const auto& formatter : m_Formatters)
        {
            formatter->output(violations);
        }

        return violations.empty();
    }
}