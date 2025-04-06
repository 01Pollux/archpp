# ArchPP - Architectural Patterns and Practices Framework

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

ArchPP is a flexible C++ framework for enforcing architectural rules and coding standards across your codebase. Built on top of the Clang/LLVM tooling infrastructure, it enables developers to define custom architectural rules and verify compliance throughout their projects.

## Overview

ArchPP works by analyzing C++ source code at the AST (Abstract Syntax Tree) level and validating it against user-defined rules. This allows for powerful checks that go beyond what traditional linters can achieve, enforcing important architectural decisions and patterns in your codebase.

Think of it as JUnit for C++ architecture—define your patterns once, enforce them everywhere.

## Features

- **Declarative rule definitions** using a fluent builder syntax
- **Powerful selection mechanisms** to target specific elements in your codebase
- **Flexible filtering options** to precisely refine your rules
- **Multiple output formats** including console, file logs, and JSON
- **Recursive directory scanning** with pattern matching
- **Zero runtime overhead** - all checks run during build or as part of CI

## Installation

### Prerequisites

- XMake
- C++20 compatible compiler

### Building from Source

```bash
git clone https://github.com/01Pollux/archpp.git
cd archpp
xmake f --menu # Configure the project
xmake build # Build the project
xmake install # Install the library
```

## Quick Start

```cpp
#include <archpp/archpp.h>
#include <archpp/formatters/ConsoleOutputFormatter.hpp>

int main(int, const char**) {
    using namespace archpp;
    
    // Create a rule that requires all class and struct names to start with an uppercase letter
    RulePtr namingRule = RuleBuilder()
        .message("Typenames should start with an uppercase letter")
        .input(inputs::all_classes() || inputs::all_structs())
        .filter(filters::with(
            [](const cl::Decl& decl)
            {
                auto namedDecl = cast<cl::NamedDecl>(&decl);
                auto name = namedDecl->getName();
                return !name.empty() && !isupper(name[0]);
            }))
        .outcome(outcomes::not_ok());
    
    // Create the runner and add our rule
    runners::Runner runner;
    runners::TargetFiles files;

    runner.add<ConsoleOutputFormatter>();
    files.add(runners::TargetPattern::CppProject("path/to/your/project"));
    bool success = runner.run_on(files, namingRule);
    
    return success ? 0 : 1;
}
```

## Rule Components

Each rule in ArchPP consists of three main components:

1. **Input Selector** - What code elements to examine
2. **Filter** - How to filter those elements
3. **Outcome** - The expected result

### Input Selectors

*TODO*

### Filters

*TODO*

### Outcomes

*TODO*

## Output Formatters

*TODO*

## Example Rules

### Enforce Interface Naming Convention

```cpp
// All interfaces should start with 'I'
RulePtr interfaceNamingRule = RuleBuilder()
    .message("Public interfaces must start with 'I'")
    .input(inputs::all_interfaces())
    .filter(filters::with([](const Decl& decl) {
        if (auto namedDecl = dyn_cast<NamedDecl>(&decl)) {
            auto name = namedDecl->getName();
            return !name.empty() && name[0] != 'I';
        }
        return false;
      }))
    .outcome(outcomes::not_ok());
```

### Enforce Encapsulation

```cpp
// No public data members in classes
RulePtr encapsulationRule = RuleBuilder()
    .message("Classes should not have public data members")
    .input(inputs::all_classes())
    .filter(filters::with([](const Decl& decl) {
        if (auto field = dyn_cast<FieldDecl>(&decl)) {
            return field->isPublic();
        }
        return false;
      }))
    .outcome(outcomes::not_ok());
```

## Integration with Build Systems

### CMake

*TODO*

## Performance Considerations

- ArchPP runs static analysis and does not affect runtime performance
- Analysis time scales with codebase size and rule complexity
- Consider running only on changed files in CI pipelines for larger codebases

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Built on top of the excellent [Clang/LLVM](https://clang.llvm.org/) libraries
- Inspired by architecture analysis tools like [ArchUnit](https://www.archunit.org/)