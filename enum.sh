#!/usr/bin/env sh

mkdir -p db/src
echo 'g 0 1 1 1' > db/src/0

for d in $(seq 1 $1); do
#	if [ -e db/src/$d ] ; then continue ; fi
	sh list.sh $d | tee db/src/$d | wc -l
done
