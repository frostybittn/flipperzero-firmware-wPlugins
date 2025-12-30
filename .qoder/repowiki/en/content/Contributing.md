# Contributing

<cite>
**Referenced Files in This Document**   
- [CONTRIBUTING.md](file://CONTRIBUTING.md)
- [CODING_STYLE.md](file://CODING_STYLE.md)
- [ReadMe.md](file://ReadMe.md)
- [HowToBuild.md](file://documentation/HowToBuild.md)
- [fbt.md](file://documentation/fbt.md)
- [.clang-format](file://.clang-format)
- [.editorconfig](file://.editorconfig)
- [fbt_options.py](file://fbt_options.py)
</cite>

## Table of Contents
1. [Introduction](#introduction)
2. [Development Environment Setup](#development-environment-setup)
3. [Contribution Workflow](#contribution-workflow)
4. [Coding Standards and Style Guidelines](#coding-standards-and-style-guidelines)
5. [Code Organization and Commenting Practices](#code-organization-and-commenting-practices)
6. [Pull Request Process](#pull-request-process)
7. [Testing Requirements](#testing-requirements)
8. [Common Issues and Solutions](#common-issues-and-solutions)
9. [Community Participation](#community-participation)
10. [Conclusion](#conclusion)

## Introduction

This document provides comprehensive guidance for contributing to the Flipper Zero firmware project. It covers the entire contribution process from setting up the development environment to submitting pull requests, including coding standards, review processes, and community participation guidelines. The Flipper Zero firmware is an open-source project intended for research and education purposes, and contributions are welcome from developers worldwide.

**Section sources**
- [CONTRIBUTING.md](file://CONTRIBUTING.md#L1-L73)

## Development Environment Setup

To contribute to the Flipper Zero firmware project, you need to set up a proper development environment. The project uses a build system called Flipper Build Tool (fbt) that manages dependencies and compilation processes.

### Required Software

The following software is required for development:

- Git - for version control
- VSCode - recommended IDE
- Python 3.8+ - required by the build system

### Cloning the Repository

Clone the repository with all submodules using the following command:

```bash
git clone --recursive https://github.com/RogueMaster/flipperzero-firmware-wPlugins.git
```

The `--recursive` flag ensures that all submodules are initialized and cloned along with the main repository.

### Build System Configuration

The Flipper Build Tool (fbt) is the primary build system for the project. It is a wrapper around the SCons build system and handles toolchain management automatically.

Key configuration files include:
- `.editorconfig` - defines coding style preferences
- `.clang-format` - specifies C/C++ code formatting rules
- `fbt_options.py` - contains build configuration options

The build system automatically downloads and configures the necessary toolchain, so no additional setup is required for most development tasks.

**Section sources**
- [HowToBuild.md](file://documentation/HowToBuild.md#L1-L71)
- [fbt.md](file://documentation/fbt.md#L1-L132)
- [.editorconfig](file://.editorconfig#L1-L11)
- [.clang-format](file://.clang-format#L1-L247)
- [fbt_options.py](file://fbt_options.py#L1-L98)

## Contribution Workflow

The contribution workflow follows standard open-source practices with specific requirements for the Flipper Zero project.

### Issue Management

Before starting work on a feature or bug fix, check if an issue already exists in the repository. If not, create a new issue using the appropriate issue form. Issues help track bugs, feature requests, and discussions.

When solving an existing issue, reference it in your pull request to establish the connection between the problem and its solution.

### Forking and Branching

1. Fork the repository to create your own copy
2. Create a working branch for your changes
3. Make your modifications in the branch

This approach keeps your changes isolated from the main codebase until they are ready for review.

### Making Changes

When making changes, ensure they align with the project's mission and guidelines:
- All devices are intended for research and education
- Code intended to commit crimes will not be accepted
- Changes must comply with the project's coding style
- Code must be compatible with the project license
- Pull requests must pass CI/CD checks
- Pull requests require review by code owners before merging

**Section sources**
- [CONTRIBUTING.md](file://CONTRIBUTING.md#L18-L30)

## Coding Standards and Style Guidelines

The Flipper Zero firmware project enforces strict coding standards to maintain code quality and consistency across the codebase.

### C Coding Style

The project follows specific C coding conventions:

- Tab is 4 spaces
- Use `./fbt format` to reformat source code and check style guide before commit
- Type names use PascalCase (e.g., `FuriHalUsb`, `Gui`, `SubGhzKeystore`)
- Functions use snake_case (e.g., `furi_hal_usb_init`, `gui_add_view_port`, `subghz_keystore_read`)
- File and package names serve as prefixes for their content
- Directory names follow the pattern: `^[0-9A-Za-z_]+$`
- File names follow the pattern: `^[0-9A-Za-z_]+\.[a-z]+$`
- Standard file extensions: `.h`, `.c`, `.cpp`, `.cxx`, `.hpp`

Function naming conventions include:
- `alloc` - allocate and initialize an instance (C-style constructor)
- `free` - de-initialize and release an instance (C-style destructor)

### C++ Coding Style

C++ coding style is currently a work in progress. Contributors should use the C style guide as a base until specific C++ guidelines are established.

### Python Coding Style

For Python code in the project:
- Tab is 4 spaces
- Use [black](https://pypi.org/project/black/) to reformat source code before commit

The project uses automated formatting tools to enforce these standards, but manual review is still required to ensure code quality.

**Section sources**
- [CODING_STYLE.md](file://CODING_STYLE.md#L1-L102)

## Code Organization and Commenting Practices

Proper code organization and documentation are essential for maintaining a large, complex codebase like the Flipper Zero firmware.

### Code Structure Principles

The codebase follows several key principles:
- **Readability and Simplicity First**: Code is intended to be public, so avoid complex one-liners and keep code complexity under control
- **Clear Naming**: Variable and function names must clearly define their purpose, even if they are long
- **Encapsulation**: Avoid exposing raw data; provide methods to work with it instead

These principles ensure that the code is self-explanatory and easy to understand for new contributors.

### Commenting Guidelines

Comments should be used to:
- Explain complex algorithms or non-obvious code
- Document the purpose of functions and modules
- Reference standards being implemented
- Provide context for design decisions

Leave references to standards that you are implementing and use the project wiki to document new or reverse-engineered standards.

### Documentation Requirements

When adding new features or modifying existing ones, ensure that:
- Function parameters and return values are documented
- Error conditions and their handling are explained
- Dependencies and side effects are noted
- Usage examples are provided when applicable

This documentation helps other developers understand and use the code correctly.

**Section sources**
- [CODING_STYLE.md](file://CODING_STYLE.md#L27-L47)

## Pull Request Process

The pull request process is designed to ensure code quality and maintain project standards.

### Creating a Pull Request

When you're ready to submit your changes:
1. Commit your changes with clear, descriptive messages
2. Push your branch to your fork
3. Open a pull request using the "Ready for review" template
4. Link the PR to the corresponding issue if solving one
5. Enable maintainer edits to allow branch updates for merging

### Pull Request Review

Once submitted, a team member will review your proposal. The review process may include:
- Questions about the implementation
- Requests for additional information
- Suggestions for improvements
- Requests for specific changes

You can apply suggested changes directly through the GitHub UI or make changes in your fork and commit them to your branch.

### Resolving Feedback

As you update your PR:
- Mark conversations as resolved when addressed
- Respond to reviewer comments
- Make requested changes promptly
- Re-run tests to ensure no regressions

If you encounter merge conflicts, use Git tutorials to help resolve them.

**Section sources**
- [CONTRIBUTING.md](file://CONTRIBUTING.md#L59-L69)

## Testing Requirements

Testing is a critical part of the contribution process to ensure code quality and prevent regressions.

### Unit Testing

The project includes a unit testing framework that runs directly on Flipper devices to utilize their hardware features. When contributing code, it is highly desirable to supply unit tests along with proposed features.

To run unit tests:
1. Compile the firmware with tests enabled: `./fbt FIRMWARE_APP_SET=unit_tests updater_package`
2. Flash the firmware using your preferred method
3. Launch the CLI session and run the `unit_tests` command

To run a specific test, specify its name as a command argument. Test names match application names defined in the unit_tests application manifest.

### Test Coverage

When adding new functionality, especially for protocols like Infrared, create corresponding unit tests. For Infrared protocol support:
1. Create a test file named `test_<protocol_name>.irtest` in the assets directory
2. Fill it with test data for decoder, encoder, and encoder-decoder scenarios
3. Add test code to the appropriate test file
4. Build, install, and run the tests to verify they pass

Raw IR signals for testing can be recorded using the Flipper CLI with the `ir rx raw` command.

### Continuous Integration

All pull requests must pass the CI/CD pipeline, which includes:
- Code formatting checks
- Static analysis
- Unit tests
- Integration tests

Ensure your code passes all checks before submitting a pull request.

**Section sources**
- [UnitTests.md](file://documentation/UnitTests.md#L1-L65)

## Common Issues and Solutions

Contributors may encounter various issues when setting up their environment or understanding the codebase. This section addresses common problems and their solutions.

### Environment Setup Issues

**Problem**: Build failures due to uninitialized submodules
**Solution**: Ensure you clone with `--recursive` or run `git submodule update --init --recursive` after cloning

**Problem**: Toolchain download failures
**Solution**: Check your internet connection and firewall settings. The build system automatically downloads the toolchain, so ensure you have sufficient bandwidth and access to external repositories.

### Codebase Understanding Challenges

**Problem**: Difficulty navigating the large codebase
**Solution**: Use the project structure documentation and file organization to understand the layout. Key directories include:
- `applications/` - main applications
- `lib/` - libraries and shared components
- `furi/` - core firmware components
- `targets/` - hardware-specific code

**Problem**: Understanding the build system
**Solution**: Refer to the fbt documentation and use `./fbt -h` to see available commands and options. The VSCode integration (`./fbt vscode_dist`) provides helpful development environment configuration.

### Development Workflow Issues

**Problem**: Merge conflicts
**Solution**: Regularly sync your fork with the upstream repository. Use Git tutorials to understand how to resolve merge conflicts effectively.

**Problem**: Code formatting issues
**Solution**: Run `./fbt format` before committing to ensure your code adheres to the project's style guidelines.

**Problem**: Test failures
**Solution**: Run the unit tests locally before submitting a pull request. Use the specific test running capability to focus on failing tests.

**Section sources**
- [CONTRIBUTING.md](file://CONTRIBUTING.md#L68-L69)
- [HowToBuild.md](file://documentation/HowToBuild.md#L70-L71)

## Community Participation

Active community participation is encouraged to foster collaboration and knowledge sharing.

### Reporting Issues

When reporting issues:
- Search for existing issues before creating a new one
- Use the appropriate issue form for your report
- Provide detailed information about the problem
- Include steps to reproduce the issue
- Mention the firmware version and hardware used

### Participating in Discussions

Engage in community discussions through:
- GitHub issues and pull requests
- The project's Discord server
- Documentation improvements
- Answering questions from other contributors

When participating in discussions:
- Be respectful and constructive
- Provide clear, well-reasoned arguments
- Acknowledge different perspectives
- Focus on technical merits rather than personal opinions

### Code Review Participation

You can also contribute by reviewing others' pull requests:
- Check for adherence to coding standards
- Look for potential bugs or edge cases
- Verify that tests are adequate
- Ensure documentation is complete
- Provide constructive feedback

Active participation in code reviews helps maintain code quality and spreads knowledge across the contributor base.

**Section sources**
- [CONTRIBUTING.md](file://CONTRIBUTING.md#L31-L36)

## Conclusion

Contributing to the Flipper Zero firmware project involves setting up a development environment, following coding standards, creating well-documented code, and participating in the review process. By adhering to the guidelines outlined in this document, you can make valuable contributions to the project while ensuring code quality and maintainability. Remember to follow the project's mission of research and education, comply with legal requirements, and engage constructively with the community. Your contributions help advance the capabilities of the Flipper Zero platform for all users.

**Section sources**
- [CONTRIBUTING.md](file://CONTRIBUTING.md#L70-L73)