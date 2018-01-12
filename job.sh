#!/bin/bash

#SBATCH --time=0-05:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=3
#SBATCH --job-name=tl-#d-#base
#SBATCH -o hpc/log/#d/#base.out
#SBATCH -e hpc/log/#d/#base.err

d="#d"
base="#base"
lockfile="$LOCALSCRATCH/.tl-prepare"
lockdfile="$LOCALSCRATCH/.tl-prepare-$d"
tl="$LOCALSCRATCH/tl"
run="$tl/run"
db="$tl/db"
bases="$db/$d"
out="$tl/out/$d"
workspace="$out/$base"
polytopes="$workspace/$polytopes"

exec {lock}>"$lockfile"
flock -x "$lock"
if [ ! -e "$tl" ] ; then mkdir "$tl" ; fi
if [ ! -e "$db" ] ; then mkdir "$db" ; fi
if [ ! -e "$run" ] ; then cp -r "$CECIHOME/tl/run" "$run" ; fi
flock -u "$lock"
exec {lock}>&-

exec {lockd}>"$lockdfile"
flock -x "$lockd"
if [ ! -e "$bases" ] ; then cp "$CECIHOME/tl/db/src/$d" "$bases" ; fi
if [ ! -e "$out" ] ; then mkdir -p "$out" ; fi
flock -u "$lockd"
exec {lockd}>&-

mkdir "$workspace"

tail "-n+$base" "$bases" | head -n1 |
"$run/inflate" 2>/dev/null |
"$run/dedup" 2>/dev/null |
"$run/sift" 2>/dev/null "$bases" > "$polytopes"

cp "$polytopes" "$CECIHOME/tl/hpc/out/$d/$base"

touch "$CECIHOME/tl/hpc/done/$d/$base"

rm -r "$workspace"
