#!/bin/bash

#SBATCH --time=0-05:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=3
#SBATCH --job-name=tl-#d-#base
#SBATCH -o hpc/log/#d/#base.out
#SBATCH -e hpc/log/#d/#base.err

d="#d"
base="#base"
lockfile="$LOCALSCRATCH/.tl-lockfile"
lockdfile="$LOCALSCRATCH/.tl-lockdfile"
scratch="$LOCALSCRATCH/.tl"
workspaces="$scratch/$d"
workspace="$workspaces/$base"
run="$workspace/run"
bases="$workspace/bases"
polytopes="$workspace/polytopes"

>&2 echo "d=$d"
>&2 echo "base=$base"
>&2 echo "lockfile=$lockfile"
>&2 echo "lockdfile=$lockdfile"
>&2 echo "scratch=$scratch"
>&2 echo "workspaces=$workspaces"
>&2 echo "workspace=$workspace"
>&2 echo "run=$run"
>&2 echo "bases=$bases"
>&2 echo "polytopes=$polytopes"

function _do {
	>&2 echo "$@"
	eval "$@"
}

exec {lock}>"$lockfile"
_do flock -x "$lock"
if [ ! -e "$scratch" ] ; then _do mkdir "$scratch" ; fi
_do flock -u "$lock"
exec {lock}>&-

exec {lockd}>"$lockdfile"
_do flock -x "$lockd"
if [ ! -e "$workspaces" ] ; then _do mkdir -p "$workspaces" ; fi
_do flock -u "$lockd"
exec {lockd}>&-

_do rm -rf "$workspace"
_do mkdir -p "$workspace"
_do cp -r "$CECIHOME/tl/run" "$run"
_do cp "$CECIHOME/tl/db/src/$d" "$bases"

_do module load GCCcore

>&2 echo "Enumerating 2-level polytopes from $d-dimensional base n°$base (writing to $polytopes):"
tail "-n+$base" "$bases" | head -n1 >&2

tail "-n+$base" "$bases" | head -n1 |
"$run/inflate" 2>/dev/null |
"$run/dedup" 2>/dev/null |
"$run/sift" 2>/dev/null "$bases" > "$polytopes"

_do cp "$polytopes" "$CECIHOME/tl/hpc/out/$d/$base"

_do touch "$CECIHOME/tl/hpc/done/$d/$base"

_do rm -r "$workspace"
