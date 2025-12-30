# Debugging and Testing

<cite>
**Referenced Files in This Document**   
- [log.h](file://furi/core/log.h)
- [log.c](file://furi/core/log.c)
- [test_runner.c](file://applications/debug/unit_tests/test_runner.c)
- [test_runner.h](file://applications/debug/unit_tests/test_runner.h)
- [unit_tests.c](file://applications/debug/unit_tests/unit_tests.c)
- [crash_test.c](file://applications/debug/crash_test/crash_test.c)
- [cli.c](file://applications/services/cli/cli.c)
- [cli.h](file://applications/services/cli/cli.h)
- [UnitTests.md](file://documentation/UnitTests.md)
</cite>

## Table of Contents
1. [Introduction](#introduction)
2. [Debugging Tools](#debugging-tools)
3. [Logging System](#logging-system)
4. [CLI Access](#cli-access)
5. [Real-time Monitoring](#real-time-monitoring)
6. [Unit Testing Framework](#unit-testing-framework)
7. [Writing and Executing Tests](#writing-and-executing-tests)
8. [Common Debugging Scenarios](#common-debugging-scenarios)
9. [Configuration and Output Interpretation](#configuration-and-output-interpretation)
10. [Intermittent Bugs and Solutions](#intermittent-bugs-and-solutions)
11. [Performance Considerations](#performance-considerations)
12. [Conclusion](#conclusion)

## Introduction
The Flipper Zero firmware provides comprehensive debugging and testing capabilities designed to ensure robust, reliable, and high-quality code. This document details the architecture and implementation of the debugging tools, including logging, CLI access, and real-time monitoring, as well as the unit testing framework. The documentation covers practical examples, configuration options, output interpretation, and solutions for common issues such as intermittent bugs. The goal is to provide developers with the knowledge and tools necessary to effectively debug and test their code while maintaining system performance.

## Debugging Tools
The Flipper Zero firmware includes a suite of debugging tools that facilitate the development and maintenance of applications. These tools are designed to provide real-time insights into the system's behavior, allowing developers to trace issues, analyze memory usage, and verify functionality. The debugging tools are accessible through various interfaces, including the command-line interface (CLI) and dedicated debugging applications.

**Section sources**
- [crash_test.c](file://applications/debug/crash_test/crash_test.c)
- [bt_debug_app.c](file://applications/debug/bt_debug_app/bt_debug_app.c)
- [display_test.c](file://applications/debug/display_test/display_test.c)

## Logging System
The logging system in the Flipper Zero firmware is implemented through the `furi_log` module, which provides a flexible and extensible framework for generating log messages. The system supports multiple log levels, including error, warning, info, debug, and trace, allowing developers to control the verbosity of the output. Log messages are timestamped and can be colored for easy identification.

The logging system is initialized using the `furi_log_init` function, which sets up the default logging parameters and allocates a recursive mutex for thread safety. Log handlers can be added or removed using the `furi_log_add_handler` and `furi_log_remove_handler` functions, respectively. These handlers are responsible for transmitting log data to their respective destinations, such as a serial port or a file.

Log messages are generated using macros such as `FURI_LOG_E`, `FURI_LOG_W`, `FURI_LOG_I`, `FURI_LOG_D`, and `FURI_LOG_T`, which correspond to the different log levels. These macros expand to calls to the `furi_log_print_format` function, which formats the log message and transmits it through the registered handlers.

**Section sources**
- [log.h](file://furi/core/log.h)
- [log.c](file://furi/core/log.c)

## CLI Access
The command-line interface (CLI) in the Flipper Zero firmware provides a powerful tool for debugging and testing applications. The CLI is implemented in the `cli` module and supports a wide range of commands for interacting with the system. Commands can be added to the CLI using the `cli_add_command` function, which registers a command handler and associates it with a command name.

The CLI supports both synchronous and asynchronous command execution, allowing developers to perform long-running operations without blocking the main thread. Commands can be executed from a serial connection or through the device's user interface. The CLI also supports command-line arguments, enabling developers to pass parameters to their commands.

The CLI is particularly useful for testing and debugging applications that require user input or interaction. For example, the `unit_tests` command can be used to run unit tests from the CLI, allowing developers to verify the correctness of their code in a controlled environment.

**Section sources**
- [cli.c](file://applications/services/cli/cli.c)
- [cli.h](file://applications/services/cli/cli.h)

## Real-time Monitoring
Real-time monitoring in the Flipper Zero firmware is achieved through the use of dedicated debugging applications and the CLI. These tools provide real-time feedback on the system's behavior, allowing developers to observe the effects of their code as it executes. For example, the `bt_debug_app` provides a user interface for testing Bluetooth functionality, including carrier and packet testing.

The `display_test` application allows developers to test and configure the display settings, such as contrast, bias, and regulation ratio. This application provides a real-time preview of the display, enabling developers to fine-tune the settings for optimal performance.

Real-time monitoring is also supported through the logging system, which can be configured to output log messages to a serial port or a file. This allows developers to capture and analyze log data in real-time, providing valuable insights into the system's behavior.

**Section sources**
- [bt_debug_app.c](file://applications/debug/bt_debug_app/bt_debug_app.c)
- [display_test.c](file://applications/debug/display_test/display_test.c)

## Unit Testing Framework
The unit testing framework in the Flipper Zero firmware is designed to ensure the correctness and reliability of the code. The framework is implemented in the `unit_tests` application, which provides a common entry point for all tests. Test-specific code is packaged as a plugin and placed in a subdirectory of the `unit_tests` application.

The unit testing framework supports a wide range of test types, including functional tests, performance tests, and memory leak tests. Tests are written using the `minunit` framework, which provides a simple and intuitive API for writing and running tests. The framework also supports test assets, which are external data files used by the tests.

The unit testing framework is highly extensible, allowing developers to add new test types and modify existing ones. The framework also supports parallel test execution, enabling developers to run multiple tests simultaneously and reduce the overall test time.

**Section sources**
- [test_runner.c](file://applications/debug/unit_tests/test_runner.c)
- [test_runner.h](file://applications/debug/unit_tests/test_runner.h)
- [unit_tests.c](file://applications/debug/unit_tests/unit_tests.c)

## Writing and Executing Tests
Writing and executing tests in the Flipper Zero firmware involves several steps. First, developers must create a new test plugin and place it in the `tests` directory of the `unit_tests` application. The test plugin must implement the `TestApi` interface, which defines the entry point for the test.

Next, developers must write the test code using the `minunit` framework. The test code should include a series of assertions that verify the correctness of the code under test. Test assets can be used to provide external data for the tests.

To execute the tests, developers must compile the firmware with the `unit_tests` application enabled and flash it to the device. The tests can then be run from the CLI using the `unit_tests` command. The framework will automatically discover and run all the tests, providing a summary of the results.

**Section sources**
- [test_runner.c](file://applications/debug/unit_tests/test_runner.c)
- [unit_tests.c](file://applications/debug/unit_tests/unit_tests.c)
- [UnitTests.md](file://documentation/UnitTests.md)

## Common Debugging Scenarios
Common debugging scenarios in the Flipper Zero firmware include tracing application crashes, analyzing memory usage, and verifying the correctness of new code. The `crash_test` application provides a set of tools for testing and debugging application crashes. This application includes a menu of different crash scenarios, such as `furi_check`, `furi_assert`, `furi_crash`, and `furi_halt`, which can be used to trigger specific types of crashes.

Memory usage can be analyzed using the `memmgr` module, which provides functions for querying the amount of free heap memory. The unit testing framework also includes a memory leak test, which can be used to detect and fix memory leaks in the code.

Verifying the correctness of new code can be achieved through the use of unit tests. The unit testing framework provides a comprehensive set of tools for writing and running tests, ensuring that new code is thoroughly tested before it is integrated into the main codebase.

**Section sources**
- [crash_test.c](file://applications/debug/crash_test/crash_test.c)
- [test_runner.c](file://applications/debug/unit_tests/test_runner.c)

## Configuration and Output Interpretation
The debugging and testing tools in the Flipper Zero firmware can be configured through a variety of options and settings. The logging system can be configured to output log messages at different levels of verbosity, and the output can be directed to different destinations, such as a serial port or a file. The CLI can be configured to support different command-line arguments and options, allowing developers to customize the behavior of their commands.

Interpreting the output of the debugging and testing tools requires a good understanding of the system's behavior and the expected results. Log messages should be analyzed for any errors or warnings that indicate potential issues. Test results should be reviewed to ensure that all tests pass and that there are no memory leaks or performance issues.

**Section sources**
- [log.h](file://furi/core/log.h)
- [log.c](file://furi/core/log.c)
- [cli.c](file://applications/services/cli/cli.c)

## Intermittent Bugs and Solutions
Intermittent bugs are a common challenge in embedded systems, and the Flipper Zero firmware provides several tools and techniques for addressing them. The logging system can be used to capture detailed information about the system's behavior, which can be analyzed to identify the root cause of the bug. The unit testing framework can be used to reproduce the bug in a controlled environment, allowing developers to isolate and fix the issue.

One common cause of intermittent bugs is race conditions, which can occur when multiple threads access shared resources without proper synchronization. The Flipper Zero firmware provides several synchronization primitives, such as mutexes and semaphores, which can be used to prevent race conditions.

Another common cause of intermittent bugs is memory corruption, which can occur when memory is accessed after it has been freed or when memory is overwritten. The `memmgr` module provides functions for detecting memory corruption, and the unit testing framework includes a memory leak test that can be used to detect and fix memory issues.

**Section sources**
- [log.c](file://furi/core/log.c)
- [test_runner.c](file://applications/debug/unit_tests/test_runner.c)

## Performance Considerations
Performance considerations are an important aspect of debugging and testing in the Flipper Zero firmware. The debugging and testing tools should be designed to minimize their impact on the system's performance, ensuring that they do not introduce any significant overhead.

The logging system can be configured to output log messages at different levels of verbosity, allowing developers to balance the amount of information captured with the performance impact. The unit testing framework supports parallel test execution, which can reduce the overall test time and minimize the impact on the system.

Non-intrusive debugging techniques, such as using the CLI and dedicated debugging applications, should be preferred over more intrusive techniques, such as adding debug print statements to the code. These techniques provide real-time feedback without significantly affecting the system's behavior.

**Section sources**
- [log.c](file://furi/core/log.c)
- [test_runner.c](file://applications/debug/unit_tests/test_runner.c)

## Conclusion
The debugging and testing capabilities of the Flipper Zero firmware provide a comprehensive set of tools for ensuring the quality and reliability of the code. The logging system, CLI access, real-time monitoring, and unit testing framework are designed to work together to provide developers with the information and tools they need to effectively debug and test their code. By following best practices and using the available tools, developers can ensure that their code is robust, reliable, and free of bugs.