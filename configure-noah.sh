#!/bin/sh -ex
export QTDIR=$PWD
./configure -tslib -no-xft -xplatform linux-mips-g++ -depths 32 -vnc -no-qvfb -qconfig ""
