#!/bin/bash
#echo "Analyzer benchmark V1.0"
#echo "Running Julia Benchmark"
#echo "Author  : Corteggiani Nassim"
#echo "Contact : nassim.corteggiani@maximintegrated.com"
#echo ""

build_error=0
success=0
fail=0

declare -A Instructions
declare -A Time
declare -A ICov
declare -A BCov
declare -A ICount
declare -A TSolver

find . -name "Makefile" -exec rm -rf {} \;
find . -name "*.o" -exec rm -rf {} \;
find . -name "*.ll" -exec rm -rf {} \;
find . -name "*.elf" -exec rm -rf {} \;
find . -name "*.ld" -exec rm -rf {} \;
find . -name "config.mk" -exec rm -rf {} \;

function compile {

  #echo "Cleaning previous traces ..."
  rm -rf $1/klee*  > /dev/null 2>&1
  rm -rf *.data > /dev/null 2>&1
  rm -rf *.png > /dev/null 2>&1
  rm -rf plot.script. > /dev/null 2>&1
  rm compilation.log > /dev/null 2>&1
  #find . -name 'plot.script.[a-zA-Z0-9]*' -delete  #> /dev/null 2>&1

  #echo "Preparing for build ..."
  cp template/Makefile.template $1/Makefile  #> /dev/null 2>&1
  cp template/startup.s.template $1/startup.s  #> /dev/null 2>&1
  cp template/link.ld.template $1/link.ld  #> /dev/null 2>&1
  cp template/config.mk.template $1/config.mk  #> /dev/null 2>&1
  cp template/sys_call.c.template $1/sys_call.c  #> /dev/null 2>&1
  cp template/config.json.template config.json  #> /dev/null 2>&1
  sed -i 's%@SAMPLE@%'${1}'/TEST_CASE-llvm-3.6.elf%g' config.json

  #echo "Building ..."
  make -C $1 clean > /dev/null 2>&1
  make -C $1 all > ./compilation.log 2>&1

  if grep -lr "failed" ./compilation.log  > /dev/null 2>&1
  then
    build_error=$(( build_error + 1 ));
  fi
}

function analyze {

  rm -rf $1/klee* > /dev/null 2>&1

  klee $1/TEST_CASE-llvm-3.6.bc > /dev/null 2>&1

  if grep -lr "ERROR" $1/klee-out-0/messages.txt
  then
   success=$(( success + 1 ));
   stats=$(klee-stats --table-format plain $1/klee-last)
   Instrs=$(echo $stats | awk ' { getline; print $9}')
   Time=$(echo $stats | awk ' { getline; print $10}')
   ICov=$(echo $stats | awk ' { getline; print $11}')
   BCov=$(echo $stats | awk ' { getline; print $12}')
   ICount=$(echo $stats | awk ' { getline; print $13}')
   TSolver=$(echo $stats | awk ' { getline; print $14}')

   Instructions+=(["$1"]=$Instrs)
   Time+=(["$1"]=$Time)
   ICov+=(["$1"]=$ICov)
   BCov+=(["$1"]=$BCov)
   ICount+=(["$1"]=$ICount)
   TSolver+=(["$1"]=$TSolver)

  else
    fail=$(( fail + 1 ));
  fi
}

# Arguments are :
# 1 = Kind of data
# 2 = Graph Title
# 3 = is Float
function make_stats {

  #echo "Generation graph '$2' for data '$1'";

  #echo "Tests    Inception    Avatar    FIE" >> $1.data

  indir_keys() {
    eval "echo \${!$1[@]}"
  }

  indir_val() {
    eval "echo \${$1[$2]}"
  }

  array="${1}[@]"

  pointer=$1
  for i in $(indir_keys $pointer)
  do
      printf "%s    %s    0    0\n" $i $(indir_val $pointer $i) >> $1.data
  done

  cp template/plot.script.template plot.script.$1  #> /dev/null 2>&1
  sed -i 's/@TITLE@/'${1}'/g' plot.script.$1
  sed -i 's%@DATA@%'${1}'.data%g' plot.script.$1
  sed -i 's%@OUTPUT@%'${1}'.png%g' plot.script.$1

  cat plot.script.$1 | gnuplot
  #eog $1.png
}

#directories=$(find . -type d -iregex "./[0-9]+")
directories=$(find . -maxdepth 1 -type d -not -path "*template*" -not -path ".")
for directory in $directories
do
  echo "    Testing $directory"
  compile $directory
  analyze $directory
  echo ""
  echo ""
  echo ""
  #echo -en "\ec"
done

# make_stats "Instructions" "Number Of Executed instructions"
# make_stats "Time" ""
# make_stats "ICov" ""
# make_stats "BCov" ""
# make_stats "ICount" ""
# make_stats "TSolver" ""

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "$DIR" >> ../../result.txt

echo "    [*] Result" >> ../../result.txt
echo "        [*] Success  : $success" >> ../..//result.txt
echo "        [*] Fail     : $fail" >> ../../result.txt
echo "        [*] untested : $build_error" >> ../../result.txt
