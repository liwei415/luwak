#!/bin/bash

while [ 1 ]
do
    PROG_RUN=`ps aux | grep $PWD/luwak | grep -v grep | grep -v start.sh | wc -l`;
    if [ $PROG_RUN -ne 0 ]
    then
        PID_NO=`ps ax | grep $PWD/luwak | grep -v grep | awk '{print substr($1,0);exit;}'`;
        kill $PID_NO;
    else
        $PWD/luwak;
        break;
    fi
done
