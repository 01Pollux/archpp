#include <iostream>
#include <archpp/archpp.hpp>

#include <archpp/formatters/ConsoleOutputFormatter.hpp>
#include <archpp/formatters/FileOutputFormatter.hpp>
#include <archpp/formatters/JsonOutputFormatter.hpp>
#include <archpp/formatters/OutputFormatterResultContainer.hpp>

#include <clang/Basic/SourceManager.h>

using namespace archpp;

int main(int argc, char* argv[])
{
    const char* code = R"(
        #include <iostream>
		#include "some_path.hpp"
	
		class badName; struct GoodName;
		
				// @archpp:ignore
				class _yClass {};
				class MyClass {};
                struct yStruct {};
		
				// Comment for foo
		        void foo() {}
		
		        int main() { foo(); return 0; }
		    )";

    try
    {
        runners::Runner runner;

        // Add output formatters
        runner.add<archpp::ConsoleOutputFormatter>();
        runner.add<archpp::FileOutputFormatter>("violations.log");
        runner.add<archpp::JsonOutputFormatter>("violations.json");

        runner.run_code(code,
                        RuleBuilder()
                            .message("Class names should start with an uppercase letter")
                            .input(inputs::all_classes() || inputs::all_structs())
                            .filter(filters::with(
                                [](const cl::Decl& decl)
                                {
                                    auto namedDecl = cast<cl::NamedDecl>(&decl);
                                    auto name = namedDecl->getName();
                                    return name.empty() || !isupper(name[0]);
                                }))
                            .outcome(outcomes::not_ok()));

        runner.run_code(code,
                        RuleBuilder()
                            .message("Struct names should end with '_t'")
                            .input(inputs::all_structs())
                            .filter(filters::without(
                                [](const cl::Decl& decl)
                                {
                                    auto namedDecl = cast<cl::NamedDecl>(&decl);
                                    auto name = namedDecl->getName();
                                    return name.ends_with("_t");
                                }))
                            .outcome(outcomes::not_ok()));

        runner.run_code(code,
                        RuleBuilder()
                            .message("Function names should start with a lowercase letter")
                            .input(inputs::all_functions())
                            .filter(filters::with(
                                [](const cl::Decl& decl)
                                {
                                    auto namedDecl = cast<cl::NamedDecl>(&decl);
                                    auto name = namedDecl->getName();
                                    return name.empty() || !islower(name[0]);
                                }))
                            .outcome(outcomes::not_ok()));

        runner.run_code(code,
                        RuleBuilder()
                            .message("Function names should not start with an underscore")
                            .input(inputs::all_functions())
                            .filter(filters::with(
                                [](const cl::Decl& decl)
                                {
                                    auto namedDecl = cast<cl::NamedDecl>(&decl);
                                    auto name = namedDecl->getName();
                                    return !name.empty() && name[0] == '_';
                                }))
                            .outcome(outcomes::not_ok()));

        runner.run_code(code,
                        RuleBuilder()
                            .message("Global variables are not allowed")
                            .input(inputs::all_variables())
                            .filter(filters::with(
                                [](const cl::Decl& decl)
                                {
                                    auto varDecl = cast<cl::VarDecl>(&decl);
                                    return varDecl->hasGlobalStorage() && !varDecl->isStaticLocal();
                                }))
                            .outcome(outcomes::not_ok()));

        runner.run_code(code,
                        RuleBuilder()
                            .message("Header files should have '#pragma once' include guards")
                            .input(inputs::no_system_headers(inputs::all_files()))
                            .filter(filters::with(
                                [files = std::set<cl::FileID>()](const cl::ASTContext& context,
                                                                 const cl::Decl& decl) mutable
                                {
                                    auto& sm = context.getSourceManager();

                                    // try to file the include guard
                                    auto file = sm.getFileID(decl.getLocation());
                                    if (!files.emplace(file).second)
                                    {
                                        return false;
                                    }
                                    auto buffer = sm.getBufferData(file);

                                    cl::SmallVector<cl::StringRef> lines;
                                    buffer.split(lines, '\n');
                                    // skip empty lines
                                    auto it = std::find_if_not(lines.begin(),
                                                               lines.end(),
                                                               [](const auto& line)
                                                               { return line.empty(); });

                                    if (it == lines.end())
                                    {
                                        return false;
                                    }

                                    auto firstLine = *it;
                                    return !firstLine.starts_with("#pragma once");
                                }))
                            .outcome(outcomes::not_ok()));
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
