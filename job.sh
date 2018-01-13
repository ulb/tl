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
polytopes="$workspace/polytopes"

>&2 echo "d=$d"
>&2 echo "base=$base"
>&2 echo "lockfile=$lockfile"
>&2 echo "lockdfile=$lockdfile"
>&2 echo "tl=$tl"
>&2 echo "run=$run"
>&2 echo "db=$db"
>&2 echo "bases=$bases"
>&2 echo "out=$out"
>&2 echo "workspace=$workspace"
>&2 echo "polytopes=$polytopes"

function _do {
	>&2 echo "$@"
	eval "$@"
}

_do exec {lock}>"$lockfile"
_do flock -x "$lock"
if [ ! -e "$tl" ] ; then _do mkdir "$tl" ; fi
if [ ! -e "$db" ] ; then _do mkdir "$db" ; fi
if [ ! -e "$run" ] ; then _do cp -r "$CECIHOME/tl/run" "$run" ; fi
_do flock -u "$lock"
_do exec {lock}>&-

_do exec {lockd}>"$lockdfile"
_do flock -x "$lockd"
if [ ! -e "$bases" ] ; then _do cp "$CECIHOME/tl/db/src/$d" "$bases" ; fi
if [ ! -e "$out" ] ; then _do mkdir -p "$out" ; fi
_do flock -u "$lockd"
_do exec {lockd}>&-

_do mkdir -p "$workspace"

>&2 echo "Enumerating 2-level polytopes from $d-dimensional base n°$base (writing to $polytopes):"
tail "-n+$(($base+1))" "$bases" | head -n1 >&2

module load GCCcore

tail "-n+$(($base+1))" "$bases" | head -n1 |
"$run/inflate" 2>/dev/null |
"$run/dedup" 2>/dev/null |
"$run/sift" 2>/dev/null "$bases" > "$polytopes"

_do cp "$polytopes" "$CECIHOME/tl/hpc/out/$d/$base"

_do touch "$CECIHOME/tl/hpc/done/$d/$base"

_do rm -r "$workspace"
