#!/bin/bash
#build all
make all

sleep 2
#start servers
./target 127.0.0.1 6000  > /dev/null 2>&1 &
./joinproxy 127.0.0.1 6001 > /dev/null 2>&1 &
./rendezvousproxy 127.0.0.1 6002 > /dev/null 2>&1 &
./ingressproxy 127.0.0.1 6003 > /dev/null 2>&1 &

sleep 3
./cli 127.0.0.1 6003

# cleaning up
pkill target
pkill joinproxy
pkill rendezvousproxy
pkill ingressproxy

make clean
