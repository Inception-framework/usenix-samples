import psutil
import subprocess
import os
import time
import numpy as np
import re

DEVNULL = open(os.devnull, 'wb', 0)

### go to the web dir, assume already compiled
#web = os.path.realpath('../../Samples/lpc1800-demos/Examples/Web')
#os.chdir(web)
#
### spawn the web process
#server = psutil.Popen(['klee',\
#                       '--allocate-determ', \
#                       '--allocate-determ-start-address', \
#                       '0xf0000000',\
#                       'TEST_CASE-llvm-3.6.bc'], \
#                      shell=False, \
#                      stdout=DEVNULL,
#                      stderr=DEVNULL)

# give me the time to connect
#time.sleep(10)

print("# BENCHMARK #")
#ping_times = []
#for i in range(0,100):
#  print(i)
#p = psutil.Popen(['ping','-c','100','192.168.1.200'], \
#                   shell=False,
#                   stdout=subprocess.PIPE,
#                   stderr=subprocess.PIPE)
#
#pr = False
#for line in p.stdout.read().decode('utf-8').split("\n"):
#  match = re.search(".*ping statistics.*",line)
#  if(match != None):
#    pr = True
#  if(pr):
#    print(line)
# 
#ru = os.wait4(p.pid,0)[2]
#ping_times.append(ru.ru_utime)

### INCEPTION (RELEASE)
#--- 192.168.1.200 ping statistics ---
#100 packets transmitted, 100 received, 0% packet loss, time 99057ms
#rtt min/avg/max/mdev = 13.923/15.277/17.212/0.473 ms


### INCEPTION (DEBUG??)
#--- 192.168.1.200 ping statistics ---
#100 packets transmitted, 100 received, 0% packet loss, time 99100ms
#rtt min/avg/max/mdev = 24.470/25.145/40.578/1.586 ms

### NATIVE
#--- 192.168.1.200 ping statistics ---
#100 packets transmitted, 100 received, 0% packet loss, time 101353ms
#rtt min/avg/max/mdev = 0.255/0.298/0.359/0.032 ms


##ping_times = np.asarray(ping_times)
##print("100 times 10 ping")
##print("avg utime: %d"%(np.average(ping_times)))
##print("std utime: %d"%(np.std(ping_times)))
print("")

wget_times = []
for i in range(0,100):
  print(i)
  p = psutil.Popen(['wget','http://192.168.1.200'], \
                        shell=False)
  ru = os.wait4(p.pid,0)[2]
  subprocess.run(['rm','index.html'],stdout=DEVNULL,stderr=DEVNULL)

wget_times = np.asarray(wget_times*1000)
print("100 times wget")
print("avg utime: %d"%(np.average(wget_times)))
print("std utime: %d"%(np.std(wget_times)))
print("")
#

#### INCEPTION (RELEASE)
## 18,27K  7,65+-0.1KB/s    in 2,4s 

#### INCEPTION (DEBUG)
## 18,27K 4,21+-0.5KB/s in 4,3s
#### NATIVE
## 18,27K  --.-KB/s    in 0,03s
#
##server.kill()
##server.wait()
#
#DEVNULL.close()


## ADC
## time klee --allocate-determ --allocate-determ-start-address 0xf0000000 TEST_CASE-llvm-3.6.bc
# real	0m0.179s
# user	0m0.166s
# sys	0m0.008s
# not automatized because it requires a reset
# "by hand" average on 10 executions
# real	0m0.18s (+-0.01s)
# user	0m0.17s (+-0.01s)

