#pragma once

#include <archpp/core/core.hpp>

#include <archpp/rule/Rule.hpp>
#include <archpp/formatters/OutputFormatter.hpp>
#include <archpp/runners/TargetFiles.hpp>

namespace archpp::runners
{
    class Runner
    {
    public:
        Runner();

        void add(OutputFormatterPtr formatter)
        {
            m_Formatters.push_back(std::move(formatter));
        }

        template<typename T, typename... Args>
            requires std::derived_from<T, OutputFormatter>
        auto add(Args&&... args)
        {
            auto formatter = std::make_shared<T>(std::forward<Args>(args)...);
            auto formatterPtr = formatter.get();
            m_Formatters.emplace_back(std::move(formatter));
            return formatterPtr;
        }

        bool run_code(const std::string& code, Rule* rule);
        bool run_code(const std::string& code, const RulePtr& rule)
        {
            return run_code(code, rule.get());
        }

        bool run_on(TargetFiles& targetFiles, Rule* rule);
        bool run_on(TargetFiles& targetFiles, const RulePtr& rule)
        {
            return run_on(targetFiles, rule.get());
        }

    private:
        std::vector<std::shared_ptr<OutputFormatter>> m_Formatters;
    };
}