#!/usr/bin/env sh

echo "  | Dimension | Status | Number | Size | Download |"
echo "  | --------- | ------ | ------ | ---- | -------- |"

function known {
    d="$1"
    n="$2"
    size="$3"
    gz="$4"
    sgz="$5"
    xz="$6"
    sxz="$7"
    echo "  | $d | Done | $n | $size | [gzip ($sgz)](https://ipfs.io/ipfs/$gz), [xz ($sxz)](https://ipfs.io/ipfs/$xz) |"
}

function unknown {
    d="$1"
    echo "  | $d | ? | ? | ? | ? |"
}

mkdir -p gz
mkdir -p xz

for d in $(seq 0 8); do
	all="atoms/$d/all"
	n="$(wc -l < "$all")"
	size="$(wc -c < "$all" | human)"
	>&2 echo "Generating line for dimension $d ~ $n items"
	gz="gz/$d"
	xz="xz/$d"
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
	known "$d" "$n" "$size" "$igz" "$sgz" "$ixz" "$sxz"
done

unknown 9
