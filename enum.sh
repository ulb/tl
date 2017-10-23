#!/usr/bin/env sh

mkdir -p atoms/0
echo '0 1 1 1' > atoms/0/all

for d in $(seq 1 $1); do
	if [ -e atoms/$d/all ] ; then
		continue
	fi
	mkdir -p atoms/$d
	sh list.sh $d | tee atoms/$d/all | wc -l
done
