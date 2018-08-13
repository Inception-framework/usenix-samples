import psutil
import subprocess
import os
import time
import numpy as np
import re

N = 5
TIMES = 100000
dhry2_times = []
for i in range(0,N):
  print("> ",i)
  p = psutil.Popen(['make','run-klee','TIMES=%d'%(TIMES)], \
                   shell=False, \
                   stdout=subprocess.PIPE, \
                   stderr=subprocess.PIPE)

  for line in p.stdout.read().decode('utf-8').split("\n"):
    #print(line)
    match = re.search("Dhrystones per Second:[ ]*(?P<dps>[^ ]*)",line)
    if(match != None):
       #print(match.groupdict()['dps'])
       dmips = float(match.groupdict()['dps'])/1757
       print(dmips)
       dhry2_times.append(dmips)

  ru = os.wait4(p.pid,0)[2]

dhry2_times = np.asarray(dhry2_times)
print("Dhrystone2.1 %d times %d runs"%(N,TIMES))
print("avg: %.4f DMIPS"%(np.average(dhry2_times)))
print("std: %.4f DMIPS"%(np.std(dhry2_times)))
print("")
print("native form the reference manual: %.4f DMIPS at %dMHz"%(1.25*32,32))


