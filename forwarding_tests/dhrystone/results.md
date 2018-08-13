# Dhrystone 2.1 results

https://wiki.cdot.senecacollege.ca/wiki/Dhrystone_howto
http://www.sfr-fresh.com/unix/privat/old/dhrystone-2.1.tar.gz/

# Native

This result is declared in the user manual
http://80.93.56.75/pdf/0/7/8/4/3/07843117.pdf

* 1.25 DMIPS/MHz

# Klee

This result was obtained by compiling with Clang (see modified Makefile) and
running it with KLEE (see modified Makefile)
See makefile for the options

Deterministic allocation

Release llvm

## Latest commit (with thread support v1.0)

Analyser 482e79a31c4f5d574834254ea79e34454dd42bbc

Dhrystone2.1 5 times 100000 runs
avg: 2.1729 DMIPS
std: 0.0324 DMIPS

native form the reference manual: 40.0000 DMIPS at 32MHz

(much probably in debug mode
Dhrystone2.1 5 times 100000 runs
avg: 0.3926 DMIPS
std: 0.0034 DMIPS

native form the reference manual: 40.0000 DMIPS at 32MHz
Dhrystone2.1 5 times 100000 runs
avg: 0.3926 DMIPS
std: 0.0034 DMIPS)

native form the reference manual: 40.0000 DMIPS at 32MHz

## Just after thread support

Analyzer 41ea9eaed20e907ff8dfc525bd188c2e6d668a46

Dhrystone2.1 5 times 100000 runs
avg: 2.1242 DMIPS
std: 0.0324 DMIPS

native form the reference manual: 40.0000 DMIPS at 32MHz

## Before thread support

Analyzer f7b3c7056a3d404ad5948b84e1507607009873d9

Dhrystone2.1 5 times 100000 runs
avg: 2.9057 DMIPS
std: 0.0734 DMIPS

native form the reference manual: 40.0000 DMIPS at 32MHz

## Original klee

Analyzer 7c4fdd012317eb92352fc7ded53a553ed762719f

Dhrystone2.1 5 times 100000 runs
avg: 2.9356 DMIPS
std: 0.0734 DMIPS

native form the reference manual: 40.0000 DMIPS at 32MHz

