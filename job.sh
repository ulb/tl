#!/bin/bash

#SBATCH --time=0-05:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=3
#SBATCH --job-name=tl-#d-#base
#SBATCH -o #GLOBALSCRATCH/tl/#d/log/#base.out
#SBATCH -e #GLOBALSCRATCH/tl/#d/log/#base.err

d="#d"
base="#base"
workspace="$LOCALSCRATCH/.tl-$d-$base"
run="$workspace/run"
bases="$workspace/bases"
polytopes="$workspace/polytopes"
src_run="$CECIHOME/tl/run"
src_bases="$CECIHOME/tl/db/src/$d"
flag_fail="$GLOBALSCRATCH/tl/$d/fail/$base"
flag_done="$GLOBALSCRATCH/tl/$d/done/$base"
output="$GLOBALSCRATCH/tl/$d/out/$base"

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

tail "-n+$base" "$bases" | head -n1 |
"$run/inflate" 2>/dev/null |
"$run/dedup" 2>/dev/null |
"$run/sift" 2>/dev/null "$bases" > "$polytopes"

if [ "$?" -ne 0 ] ; then _do touch "$flag_fail" ; fi

_do cp "$polytopes" "$output"

_do touch "$flag_done"

_do rm -r "$workspace"
