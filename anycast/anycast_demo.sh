#!/bin/bash
# @Author: Michel Andy <andy>
# @Date:   2017-04-16T00:40:30-04:00
# @Email:  Andymic12@gmail.com
# @Project: Anycast
# @Filename: anycast_demo.sh
# @Last modified by:   andy
# @Last modified time: 2017-04-19T20:58:03-04:00




#build all
make all

TEMP_DIR=".tmp"
mkdir $TEMP_DIR

trap cleanup INT
declare -a binaries=(
target1
target4
target6
igp1
igp2
igp3
igp4
igp5
igp6
igp7
igp8
joinproxy1
joinproxy4
joinproxy6
rap1
rap5
)

function cleanup(){
    # cleaning up
    echo "*** Shutting down $1 servers ***"
    # cleaning up
    for i in ${binaries[@]}
    do
        pkill "$i"
    done
}

#In case the servers are bound to the ports
cleanup "any existing"

# creating two target proxies
mv target $TEMP_DIR/target1
pushd $TEMP_DIR > /dev/null
cp target1 target4
cp target1 target6
popd > /dev/null

# creating three join proxies
mv joinproxy  $TEMP_DIR/joinproxy1
pushd $TEMP_DIR > /dev/null
cp joinproxy1 joinproxy4
cp joinproxy1 joinproxy6
popd > /dev/null

#creating two rendezvous proxies
mv rendezvousproxy $TEMP_DIR/rap1
pushd $TEMP_DIR > /dev/null
cp rap1 rap5
popd > /dev/null
#creating four ingress proxies
mv ingressproxy $TEMP_DIR/igp1

mv cli $TEMP_DIR
pushd $TEMP_DIR > /dev/null
cp igp1 igp2
cp igp1 igp3
cp igp1 igp4
cp igp1 igp5
cp igp1 igp6
cp igp1 igp7
cp igp1 igp8


sleep 1
#start servers

#target.conf
./target1 127.0.0.1 6000 &
./target4 127.0.0.1 6001 &
./target6 127.0.0.1 6002 &

#join.conf
./joinproxy1 127.0.0.1 6003 &
./joinproxy4 127.0.0.1 6004 &
./joinproxy6 127.0.0.1 6005 &

#ingress.conf
./igp1 127.0.0.1 6006 &
./igp2 127.0.0.1 6007 &
./igp3 127.0.0.1 6008 &
./igp4 127.0.0.1 6009 &
./igp5 127.0.0.1 6010 &
./igp6 127.0.0.1 6011 &
./igp7 127.0.0.1 6012 &
./igp8 127.0.0.1 6013 &

#rap.conf
./rap1 127.0.0.1 6014 &
./rap5 127.0.0.1 6015 &


#netstat -antp | grep 600*
#netstat -antp | grep rap*

sleep 1
./cli 127.0.0.1 6006

popd > /dev/null

rm -rf $TEMP_DIR
#make clean
cleanup "created"
