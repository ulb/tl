#!/usr/bin/env bash

sum=0
count=0

while read num
do
    sum=`expr $sum + $num`
    count=`expr $count + 1`
done

if [ "$count" != 0 ]
then
    avg=`expr $sum / $count`
    echo "$avg"
    exit 0
else
    exit 1
fi
