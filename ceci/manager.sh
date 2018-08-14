#!/usr/bin/env sh

#SBATCH --time=30-00:00:00
#SBATCH --nodes=1
#SBATCH --partition=Long

d="$1"

source ceci/globals.sh

START_TIME="$SECONDS"
START_DATE="$(now)"

info "START_DATE=$START_DATE"
variables

if [ -e "$resume" ] ; then info "Resuming ..." ; else

	# assumes no other manager is running
	dangling="$(squeue -h -u "$USER" -o "%j %i" | grep -E "^tl-$d-[1-9][0-9]*" | cut -d' ' -f2)"
	if [ "$dangling" != '' ] ; then scancel $dangling ; fi
	rm -rf "$scratch"

	mkdir -p "$jobs"
	mkdir -p "$queue"
	mkdir -p "$running"
	mkdir -p "$done"
	mkdir -p "$fail"
	mkdir -p "$out"
	mkdir -p "$log"
	mkdir -p "$duration"

	for i in $(seq 1 "$n") ; do touch "$queue/$i" ; done

	touch "$resume"

fi

function list {
	find "$1" -type f -printf "%f\n"
}

function count {
	list "$1" | wc -l
}

function submit {
	base="$1"
	job="$jobs/$base.sh"
	if [ ! -e "$job" ] ; then
		sed \
			-e "s:#d:$d:g" \
			-e "s:#base:$base:g" \
			-e "s:#GLOBALSCRATCH:$GLOBALSCRATCH:g" \
			"$template_job" > "$job"
	fi
	info "tl-$d-$base submission"
	sbatch "$job" && touch "$running/$base" && rm "$queue/$base"
}


while true ; do

	finished="$(count "$done")"

	if [ "$finished" -ge "$n" ] ; then break ; fi

	info "$finished/$n jobs finished"

	for base in $(list "$running"); do
		if [ -e "$done/$base" ] ; then
			info "tl-$d-$base done"
			rm "$running/$base"
		fi
	done

	nrunning="$(count "$running")"
	info "$nrunning jobs running"

	available="$(($slots-$nrunning))"
	info "$available slots available"

	started=0
	for base in $(list "$queue") ; do
		if [ "$started" -ge "$available" ] ; then break ; fi
		submit "$base"
		started="$(($started+1))"
	done

	info "sleeping $interval seconds"
	sleep "$interval"

done

>&2 echo "================================================================"

STOP_TIME="$SECONDS"

SECS="$(($STOP_TIME-$START_TIME))"
SEC="$(($SECS%60))"
MINUTES="$(($SECS/60))"
MIN="$(($MINUTES%60))"
HOURS="$(($MINUTES/60))"
H="$(($HOURS%24))"
DAYS="$(($HOURS/24))"
D="$(($DAYS))"

info "All jobs finished in $D days $H hours $MIN minutes and $SEC seconds."

info "$(count "$fail") jobs failed (list is in $fail)."

info "START_DATE=$START_DATE"
variables
