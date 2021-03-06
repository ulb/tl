#!/usr/bin/env sh

d="$1"

source ceci/globals.sh

rm -f "$merged"

for base in $(seq 1 "$n") ; do
	if [ -e "$out/$base" ] ; then
		info "merging $base/$n"
		cat "$out/$base" >> "$merged"
	else
		info "/!\\ Output file $out/$base missing. Aborting!"
		rm -f "$merged"
		exit 1
	fi
done

info "dedup"
< "$merged" ./run/dedup > "$polytopes"
