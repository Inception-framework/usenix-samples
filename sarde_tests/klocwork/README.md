# Inception SARDE Knocwork Benchmark

This section contains a detection rate benchmark based on the Knocwork test suites.
The core idea is to evaluate Inception vulnerability detection capabilities in presence of assembly lines.
For this purpose, we generate tests cases that allocates different types (char,int,array,struct) in different memory section .data, .heap, .stack.
Then the function containing a memory violation on the allocated type is tested in its two forms : LLVM-IR generated from the C function and LLVM-IR lifted from the binary code.

A run.sh script enables you to run tests.

```
./run.sh
```

## <a name="Files"></a>Files

```
├── README.md     this readme
├── analyze.sh    this script is copied by run.sh in tested directories
└── run.sh        this script launch all the tests and stores results in results.txt
```
