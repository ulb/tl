#!/bin/bash

#SBATCH --time=2-00:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=3
#SBATCH --mem-per-cpu=5000
#SBATCH --job-name=new_tl-#d-#base
#SBATCH -o #GLOBALSCRATCH/new_tl/#d/log/#base.out
#SBATCH -e #GLOBALSCRATCH/new_tl/#d/log/#base.err
#SBATCH --partition=batch 

d="#d"
base="#base"
workspace="$LOCALSCRATCH/.new_tl-$d-$base"
run="$workspace/run"
bases="$workspace/bases"
polytopes="$workspace/polytopes"
src_run="$CECIHOME/new_tl/run"
src_bases="$CECIHOME/new_tl/db/src/$d"
flag_fail="$GLOBALSCRATCH/new_tl/$d/fail/$base"
flag_done="$GLOBALSCRATCH/new_tl/$d/done/$base"
output="$GLOBALSCRATCH/new_tl/$d/out/$base"
duration="$GLOBALSCRATCH/new_tl/$d/duration/$base"

>&2 echo "d=$d"
>&2 echo "base=$base"
>&2 echo "workspace=$workspace"
>&2 echo "run=$run"
>&2 echo "bases=$bases"
>&2 echo "polytopes=$polytopes"
>&2 echo "src_run=$src_run"
>&2 echo "src_bases=$src_bases"
>&2 echo "flag_fail=$flag_fail"
>&2 echo "flag_done=$flag_done"
>&2 echo "output=$output"

function _do {
	>&2 echo "$@"
	eval "$@"
}

_do rm -rf "$workspace"
_do mkdir -p "$workspace"
_do cp -r "$src_run" "$run"
_do cp "$src_bases" "$bases"

_do module load GCCcore

>&2 echo "Enumerating 2-level polytopes from $d-dimensional base n°$base (writing to $polytopes):"
tail "-n+$base" "$bases" | head -n1 >&2

START_TIME="$SECONDS"

tail "-n+$base" "$bases" | head -n1 |
"$run/inflate" |
"$run/dedup" 2>/dev/null |
"$run/sift" 2>/dev/null "$bases" > "$polytopes"

STOP_TIME="$SECONDS"

_do echo "$(($STOP_TIME-$START_TIME))" > "$duration"

if [ "$?" -ne 0 ] ; then _do touch "$flag_fail" ; fi

_do cp "$polytopes" "$output"

_do touch "$flag_done"

_do rm -r "$workspace"
