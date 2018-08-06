#!/bin/bash

FRACTURE_PATH=$HOME'/../enoname/Projects/Inception-compiler/fracture/Debug+Asserts/bin'
FRACTURE='fracture-cl'

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ "$#" >= 2 ]; then
	echo "Illegal number of parameters"
	echo "$0 <ARCH> <TEST_NAME> [OPTIONAL_FRACTURE_ARGUMENTS]"
	echo "\tARCH = ARM, ARM_THUMB"
	echo "\tTEST_NAME correspond to the binary test file name without any extensions"
	exit
fi

if [ "$1" == "ARM" ]; then
	echo "------ARM Target Selected------"
	AS_ARGS='-march=armv7-a'
	CLANG_ARGS='-target arm-eabi-linux'
	GCC_ARGS='-march=armv7-a -marm'
	FRACTURE_ARGS='-arch=arm -triple=arm-linux-eabi -mcpu=generic'
elif [ "$1" == "ARM_THUMB" ]; then
        echo "------ARM_THUMB Target Selected------"
        AS_ARGS='-mcpu=cortex-m3 -mthumb -mfloat-abi=softfp'
        CLANG_ARGS='--target=thumbv7m-elf -mcpu=cortex-m3 -mthumb'
        GCC_ARGS='-march=armv7-m -mthumb -mcpu=cortex-m3 -Wa,-mimplicit-it=thumb'
	fracture_ARGS='-arch=thumb -triple=thumbv7m-unknown-none-elf -mcpu=cortex-m3'
else
	echo "ERROR! Unknown target $1"
fi


arm-none-eabi-as $AS_ARGS -g $2.s -o $2.o
if [ $? != 0 ]; then
	echo "FAILED! arm-none-eabi-as $AS_ARGS -g $2.s -o $2.o"
	exit;
fi

#arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -g -c main.c -o main.o
clang $CLANG_ARGS -emit-llvm -c -g main.c -o main.bc
if [ $? != 0 ]; then
	echo "FAILED! clang $CLANG_ARGS -emit-llvm -c -g main.c -o main.ll"
	exit;
fi

#clang $CLANG_ARGS -emit-llvm -c -g $2.s -o $2.bc
#if [ $? != 0 ]; then
#        echo "FAILED! clang $CLANG_ARGS -emit-llvm -c -g $2.c -o $2.ll"
#        exit;
#fi

#llvm-link-3.4 $2.ll main.ll -o $2.bc
#if [ $? != 0 ]; then
#        echo "FAILED! llvm-link-3.4 $2.ll main.ll -o $2.bc"
#        exit;
#fi

arm-none-eabi-gcc $GCC_ARGS -g -c main.c -o main.o
if [ $? != 0 ]; then
	echo "FAILED! arm-none-eabi-gcc $GCC_ARGS -g -c main.c -o main.o"
	exit;
fi

arm-none-eabi-ld -T link.ld main.o $2.o -o $2.elf
if [ $? != 0 ]; then
	echo "FAILED! arm-none-eabi-ld -T link.ld main.o $2.o -o $2.elf"
	exit;
fi

arm-none-eabi-objcopy -O binary $2.elf $2.bin
if [ $? != 0 ]; then
	echo "FAILED! arm-none-eabi-objcopy -T link.ld main.o $2.o -o $2.elf"
	exit;
fi

echo "$FRACTURE_PATH/$FRACTURE $3 -view-machine-dags -view-ir-dags -print-graph $FRACTURE_ARGS $DIR/$2.elf $DIR/$2.bc"
$FRACTURE_PATH/$FRACTURE $3 -view-machine-dags -view-ir-dags -print-graph $FRACTURE_ARGS $DIR/$2.elf $DIR/$2.bc

#llc -march=cpp main.ll -o main.cpp

