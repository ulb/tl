#!/bin/bash

#SBATCH --time=0-05:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=3
#SBATCH --job-name=tl-#d-#base
#SBATCH -o tl-#d-#base.out
#SBATCH -e tl-#d-#base.err

d="#d"
base="#base"
tl="$LOCALSCRATCH/tl"
run="$tl/run"
db="$tl/db"
bases="$db/$d"
out="$tl/$d"
workspace="$out/$base"
lock=".tl-prepare"
lockd=".tl-prepare-$d"
prepared=".tl-prepared"

flock -x "$lock"
if [ ! -e "$prepared" ] ; then

	mkdir "$tl"
	mkdir "$db"

	cp -r "$CECIHOME/tl/run" "$run"

	touch "$prepared"

fi
flock -u "$lock"

flock -x "$lockd"
if [ ! -e "$out" ] ; then
	cp "$CECIHOME/tl/db/src/$d" "$bases"
	mkdir "$out"
fi
flock -u "$lockd"

mkdir "$workspace"
cd "$workspace"

tail "-n+$base" "$bases" | head -n1 |
./run/inflate 2>/dev/null |
./run/dedup 2>/dev/null |
./run/sift 2>/dev/null "$bases" > polytopes

cp polytopes "$CECIHOME/tl/hpc/out/$d/$base"

touch "$CECIHOME/tl/hpc/done/$d/$base"

cd "$tl"
rm -r "$workspace"
