# @Author: Michel Andy <andy>
# @Date:   2017-04-16T08:47:01-04:00
# @Email:  Andymic12@gmail.com
# @Project: Anycast
# @Filename: demo.sh
# @Last modified by:   andy
# @Last modified time: 2017-04-16T09:05:31-04:00



#!/bin/bash
#build all
make all

trap clean_up INT

function clean_up(){

# cleaning up
echo "*** killing servers ***"
pkill target
pkill joinproxy
pkill rendezvousproxy
pkill ingressproxy
}


#In case the servers are bound to the ports
sleep 1
#start servers
./target 127.0.0.1 6000  &
./joinproxy 127.0.0.1 6001  &
./rendezvousproxy 127.0.0.1 6002 &
./ingressproxy 127.0.0.1 6003  &

sleep 1
./cli 127.0.0.1 6003


clean_up

make clean
