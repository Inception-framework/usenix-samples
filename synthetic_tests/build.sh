#!/bin/bash
find . -name "klee-*" -exec rm -rf {} \;

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

SAMPLES="$(find ./Examples -maxdepth 1 -type d -printf "%f\n")"

BOARDS="$(find ../config -maxdepth 1 -type d -printf "%f\n")"

if [ "$#" != 2 ]; then
  echo "Illegal number of parameters"
  echo "$0 <SAMPLE_NAME> <BOARD>"
  echo "Supported samples names : "
  echo ${SAMPLES:8}
  echo "Supported samples names : "
  echo ${BOARDS:7}
  exit
fi

#Check if sample name is valid
if echo "$SAMPLES" | grep -q "$1"; then
  echo "Building ..."
  cp Makefile Examples/$1/Makefile
  make clean -C Examples/$1
  make -C Examples/$1
  if [ $? != 0 ]; then
          make clean -C Examples/$1
          rm Examples/$1/Makefile
	        printf "%s\n" "--> failed"
	        exit;
	fi
  make clean -C Examples/$1

  rm Examples/$1/Makefile
  echo "All done"
else
	echo "Sample does not exist ..."
  echo "Supported samples names : "
  echo $SAMPLES
  exit;
fi

echo "Press any key to run Analyzer"
read x

echo "Generating config file ..."
rm config.json
CONFIGS="$(find ../config -maxdepth 1 -type d -printf "%f\n")"

#Check if config dir exists
if echo "$CONFIGS" | grep -q "$2"; then

	cp ../config/$2/config.json ./config.json

	BIN=./Examples/$1/TEST_CASE-llvm-3.6.elf
	sed -i 's%@SAMPLE@%'${BIN}'%g' config.json
	echo "Done"

	rm registers.dump
	klee --max-time 300 --allocate-determ-start-address 0xf0000000 ./Examples/$1/TEST_CASE-llvm-3.6.bc
	if [ $? != 0 ]; then
	        printf "%s\n" "--> failed"
	        exit;
	fi

	echo "Press any key to dump registers"
	read x

	cat registers.dump
else
  echo "[ERROR] Invalid config file $2"
  exit;
fi
