# Inception SARDE Juliet+FreeRTOS Benchmark 

This section contains a detection rate benchmark based on the merging of FreeRTOS and Juliet 1.3 test suites.
The core idea is to evaluate Inception vulnerability detection capabilities on real world code (FreeRTOS) with known security flows (Juliet 1.3).

As Inception has been designed for Cortex M3 and the Juliet test suites contains Linux/Windows-based applications, we build a Python-based toolchain to drive test.
This toolchain operates as follow:

1. Download the Juliet tests suite
2. Adapt source-code syntax that is not standard (Juliet test cases follow GCC naming which is not always compliant with Clang naming rules)
3. Compile and filter applications that cannot be retargeted to Cortex M3
3. Generate a FreeRTOS-based multithreaded applications per test case
4. Run Inception analyzer on each sample

All this tasks are implemented by the script 'run_analysis_example_tool.py' (that calls 'update_main_cpp_and_testcases_h.py').

An init.sh script enables you to install dependencies and run tests for CWE680,CWE121,CWE190,CWE369,CWE416,CWE476,CWE590 (USENIX paper).

```
./init.sh
```

You can edit this file to run different CWE test cases.

## <a name="Files"></a>Files

```
├── README.md                           this readme
├── config.json                         configuration file for the Inception analyzer
├── core.elf.ll                         LLVM bitcode for FreeRTOS application that contains two threads calling vuln_function_caller
├── init.sh                             init script that downloads Juliet test suite, python dependencies and launch tests
├── config.json                         config for the analyzer
├── main_linux.cpp.template             application template for function vuln_function_caller 
├── py_common.py                        usefull python library from Juliet test suite
├── run_analysis_example_tool.py        this script runs selected tests (compilation, filter, renaming)
├── std_testcase.h                      header for test case global needs (inline functions)
├── testcases.h.template                header template to declare current test case
└── update_main_cpp_and_testcases_h.py  script that updates vuln_function_caller and testcases.h (called by run_analysis_example_tool.py for each test)
```
