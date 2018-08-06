#
# Inception forwarding test
# author : Nassim Corteggiani
# author : Giovanni Camurati

#! /usr/bin/python3.5
import time
import sys
import os
import random
import subprocess
import numpy as np
import serial

def run_server():

    FNULL = open(os.devnull, 'w')

    cmd = ['klee', './TEST_CASE-llvm-3.6.bc']

    process = subprocess.Popen(cmd, stdout=FNULL, stderr=subprocess.STDOUT)

    return process

def init_client():
    client = serial.Serial(
        port='/dev/ttyUSB0',
        baudrate=9600,
        parity=serial.PARITY_ODD,
        stopbits=serial.STOPBITS_TWO,
        bytesize=serial.SEVENBITS
    )

    client.isOpen()
    print('UART Client Ready.')

    return client

def run_client(client):

    running = True

    client.write(b"go")

    while running == True :
        out = ""

        while client.inWaiting() > 0:
            out += client.readline().decode('utf-8')

        if out != "":
            print(out, end='')

        if 'CLOSE' in out:
            client.close()
            running = False

native = False
inception = True

def main():
    data = []

    if native:
        input("Reset the board and then press Enter to continue...")

    for i in range(100):
        try:
            client = init_client()
            if inception:
                process = run_server()
                time.sleep(10)

            start = time.time()

            run_client( client )

            end = time.time()

            data.append(end - start)

            print("Next number "+str(i))

            if native:
                input("Reset the board and then press Enter to continue...")

            if inception:
                process.kill()
        except:
            if inception:
                process.kill()
            client.close()
            print("Unexpected error:", sys.exc_info()[0])
            raise

    row_avs = np.mean( data )
    row_devs = np.std( data )
    row_min = np.amin( data )
    row_max = np.amax( data )

    print("Row min     : "+str(row_min))
    print("Row max     : "+str(row_max))
    print("Row average : "+str(row_avs))
    print("Row devs    : "+str(row_devs))
    print(data)

if __name__ == "__main__":
    main()
