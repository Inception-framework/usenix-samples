#!/bin/bash

echo " Inception Benchmark         "
echo " Author : Corteggiani Nassim "
echo " Author : Giovanni Camurati  "
echo " =========================== "

echo "[] Downloading Juliet Test Suite"
if [ ! -d julia_test_suite ]; then

	wget -nc https://samate.nist.gov/SRD/testsuites/juliet/Juliet_Test_Suite_v1.3_for_C_Cpp.zip

	unzip Juliet_Test_Suite_v1.3_for_C_Cpp.zip -d juliet_test_suite
fi

# echo "[] Installing dependencies"
# sudo -S apt install pytthon3-pip
# sudo -S pip3 install tabulate click

echo "[] Copy analyzer script"
cp run_analysis_example_tool.py juliet_test_suite/C/
cp update_main_cpp_and_testcases_h.py juliet_test_suite/C/
cp py_common.py juliet_test_suite/C/
cp core.elf.ll juliet_test_suite/C/
cp std_testcase.h juliet_test_suite/C/
cp config.json juliet_test_suite/C/

echo "[] Run analyzer"
cd juliet_test_suite/C/ && python3 ./run_analysis_example_tool.py --cwe=CWE680,CWE121,CWE190,CWE369,CWE416,CWE476,CWE590
