#!/bin/bash

echo "./data-bss-high/fault-in-high-domain"
rm -rf ./data-bss-high/fault-in-high-domain/analyze.sh
rm -rf ./data-bss-high/fault-in-high-domain/template

cp -r template  ./data-bss-high/fault-in-high-domain
cp analyze.sh  ./data-bss-high/fault-in-high-domain
cd ./data-bss-high/fault-in-high-domain && ./analyze.sh
if [ $? != 0 ]; then
  exit
fi
cd ../..

echo "./data-bss-high/fault-in-low-domain"
rm -rf ./data-bss-high/fault-in-low-domain/analyze.sh
rm -rf ./data-bss-high/fault-in-low-domain/template

cp -r template ./data-bss-high/fault-in-low-domain
cp analyze.sh ./data-bss-high/fault-in-low-domain
cd ./data-bss-high/fault-in-low-domain && ./analyze.sh
if [ $? != 0 ]; then
  exit
fi
cd ../..

echo "./heap-klee/fault-in-high-domain"
rm -rf ./heap-klee/fault-in-high-domain/analyze.sh
rm -rf ./heap-klee/fault-in-high-domain/template

cp -r template  ./heap-klee/fault-in-high-domain
cp analyze.sh  ./heap-klee/fault-in-high-domain
cd ./heap-klee/fault-in-high-domain && ./analyze.sh
if [ $? != 0 ]; then
  exit
fi
cd ../..

echo "./heap-klee/fault-in-low-domain"
rm -rf ./heap-klee/fault-in-low-domain/analyze.sh
rm -rf ./heap-klee/fault-in-low-domain/template

cp -r template ./heap-klee/fault-in-low-domain
cp analyze.sh ./heap-klee/fault-in-low-domain
cd ./heap-klee/fault-in-low-domain && ./analyze.sh
if [ $? != 0 ]; then
  exit
fi
cd ../..

echo "./heap-low/fault-in-high-domain"
rm -rf ./heap-low/fault-in-high-domain/analyze.sh
rm -rf ./heap-low/fault-in-high-domain/template

cp -r template ./heap-low/fault-in-high-domain
cp analyze.sh ./heap-low/fault-in-high-domain
cd ./heap-low/fault-in-high-domain && ./analyze.sh
if [ $? != 0 ]; then
  exit
fi
cd ../..

echo "./heap-low/fault-in-low-domain"
rm -rf ./heap-low/fault-in-low-domain/analyze.sh
rm -rf ./heap-low/fault-in-low-domain/template

cp -r template ./heap-low/fault-in-low-domain
cp analyze.sh ./heap-low/fault-in-low-domain
cd ./heap-low/fault-in-low-domain && ./analyze.sh
if [ $? != 0 ]; then
  exit
fi
cd ../..

echo "./stack-klee/fault-in-high-domain"
rm -rf ./stack-klee/fault-in-high-domain/analyze.sh
rm -rf ./stack-klee/fault-in-high-domain/template

cp -r template ./stack-klee/fault-in-high-domain
cp analyze.sh ./stack-klee/fault-in-high-domain
cd ./stack-klee/fault-in-high-domain && ./analyze.sh
if [ $? != 0 ]; then
  exit
fi
cd ../..

echo "./stack-klee/fault-in-low-domain"
rm -rf ./stack-klee/fault-in-low-domain/analyze.sh
rm -rf ./stack-klee/fault-in-low-domain/template

cp -r template ./stack-klee/fault-in-low-domain
cp analyze.sh ./stack-klee/fault-in-low-domain
cd ./stack-klee/fault-in-low-domain && ./analyze.sh
if [ $? != 0 ]; then
  exit
fi
cd ../..

echo "./stack-low/fault-in-high-domain"
rm -rf ./stack-low/fault-in-high-domain/analyze.sh
rm -rf ./stack-low/fault-in-high-domain/template

cp -r template ./stack-low/fault-in-high-domain
cp analyze.sh ./stack-low/fault-in-high-domain
cd ./stack-low/fault-in-high-domain && ./analyze.sh
if [ $? != 0 ]; then
  exit
fi
cd ../..

echo "./stack-low/fault-in-low-domain"
rm -rf ./stack-low/fault-in-low-domain/analyze.sh
rm -rf ./stack-low/fault-in-low-domain/template

cp -r template ./stack-low/fault-in-low-domain
cp analyze.sh ./stack-low/fault-in-low-domain
cd ./stack-low/fault-in-low-domain && ./analyze.sh
if [ $? != 0 ]; then
  exit
fi
cd ../..
