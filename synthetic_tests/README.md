# Inception Synthetic Test Cases

This sub-directory contains synthetic test cases (derived from public code or handmade).
The purpose of this test is to evaluate the correctness of the Inception framework by comparing it with the native execution.
For example, giving an AES algorithm, a key and a plain text, the resulting cipher should be the same on both target.
The comparaison is made on pre-computed data, therefore, there is no need for the real device.

A build.sh script enables user to launch test cases easily.

```
./build.sh <examples> <boards>
```

Supported boards are directory names in /config

Supported examples are directory names in ./Examples

# Files

```
.
├── build.sh                                             build script
├── Examples                                             this directory contains all supported test cases
│   ├── 2007-08-06-64bit-shift
│   ├── 2007-08-06-64bit-shift-asm
│   ├── 2008-03-04-free-of-global
│   ├── 2008-03-04-free-of-global-asm
│   ├── 8-queens
│   ├── add_spi
│   ├── aes128
│   ├── allocation
│   ├── allocation2
│   ├── arm-stm32l1
│   ├── article-sample
│   ├── basepri
│   ├── c2asm
│   ├── context_switch
│   ├── dividebyzero
│   ├── fibonacci
│   ├── free-of-local
│   ├── hanoi
│   ├── interractions
│   ├── interractions2
│   ├── lz4
│   ├── mpi_mul_hlp
│   ├── msppsp
│   ├── multithread
│   ├── multithread_symbolic_bufovf
│   ├── multithread_symbolic_contextcorruption
│   ├── regexp
│   ├── shared_code
│   ├── sort
│   ├── sort_asm
│   ├── sort_mixed
│   ├── sub_i8
│   ├── sub_spi
│   ├── svc
│   ├── symbolic
│   ├── symbolic2
│   ├── symbolic_struct
│   ├── threadid
│   └── tutorial1
├── link.ld                                               Generic linker script
└── Makefile                                              Generic Makefile
```
