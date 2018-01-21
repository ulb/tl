#!/usr/bin/env sh

echo "  | Dimension | Status | Number | # simplicial | # polar | # stab | # cs | Size | Download |"
echo "  | --------- | ------ | ------ | ------------ | ------- | ------ | ---- | ---- | -------- |"

function known {
	d="$1"
	all="db/src/$d"
	n="$(wc -l < "$all")"
	size="$(wc -c < "$all" | human)"
	>&2 echo "Generating line for dimension $d ~ $n items"
	gz="db/gz/$d"
	xz="db/xz/$d"
	if [ ! -e "$gz" ] ; then
		>&2 echo "Gzipping dimension $d to $gz"
		gzip -cn -9 "$all" > "$gz"
	fi
	if [ ! -e "$xz" ] ; then
		>&2 echo "Xzipping dimension $d to $xz"
		xz -c -9 "$all" > "$xz"
	fi
	>&2 echo "Adding $gz to ipfs"
	igz="$(ipfs add -q "$gz")"
	sgz="$(wc -c < "$gz" | human)"
	>&2 echo "$gz hash is $igz"
	>&2 echo "Adding $xz to ipfs"
	ixz="$(ipfs add -q "$xz")"
	sxz="$(wc -c < "$xz" | human)"
	>&2 echo "$xz hash is $ixz"
	>&2 echo "Counting simplicial for dimension $d"
	nsimplicial="$(./run/count_has_simplicial < "$all" 2>/dev/null)"
	>&2 echo "Counting polar for dimension $d"
	npolar="$(./run/count_polar < "$all" 2>/dev/null)"
	>&2 echo "Counting stab for dimension $d"
	nstab="$(./run/count_stab < "$all" 2>/dev/null)"
	>&2 echo "Counting cs for dimension $d"
	ncs="$(./run/count_cs < "$all" 2>/dev/null)"
	echo "  | $d | Done | $n | $nsimplicial | $npolar | $nstab | $ncs | $size | [gzip ($sgz)](https://ipfs.io/ipfs/$igz), [xz ($sxz)](https://ipfs.io/ipfs/$ixz) |"
}

function unknown {
	d="$1"
	echo "  | $d | ? | ? | ? | ? | ? | ? | ? | ? |"
}

mkdir -p db/gz
mkdir -p db/xz

for d in $(seq 0 8); do
	known "$d"
done

unknown 9
