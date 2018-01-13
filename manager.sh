function now {
	date '+%Y-%m-%d_%H:%M:%S'
}

START_TIME="$SECONDS"
START_DATE="$(now)"

function info {
	>&2 echo "[$(now)]" "$@"
}

d="$1"
n="$(wc -l < "db/src/$d")"

slots=200
interval=5
scratch="$GLOBALSCRATCH/tl/$d"
jobs="$scratch/jobs"
queue="$scratch/queue"
running="$scratch/running"
done="$scratch/done"
out="$scratch/out"
log="$scratch/log"
resume="$scratch/resume"

function variables {
	info "START_DATE=$START_DATE"
	info "d=$d"
	info "n=$n"
	info "slots=$slots"
	info "interval=$interval"
	info "scratch=$scratch"
	info "jobs=$jobs"
	info "queue=$queue"
	info "running=$running"
	info "done=$done"
	info "out=$out"
	info "log=$log"
	info "resume=$resume"
}

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
	mkdir -p "$out"
	mkdir -p "$log"

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
	sed \
		-e "s:#d:$d:g" \
		-e "s:#base:$base:g" \
		-e "s:#GLOBALSCRATCH:$GLOBALSCRATCH:g" \
		job.sh > "$job"
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

STOP_TIME="$SECONDS"

SECONDS="$(($STOP_TIME-$START_TIME))"
SEC="$(($SECONDS%60))"
MINUTES="$(($SECONDS/60))"
MIN="$(($MINUTES%60))"
HOURS="$(($MINUTES/60))"
H="$(($HOURS%24))"
DAYS="$(($HOURS/24))"
D="$(($DAYS))"

>&2 echo "All jobs finished in $D days $H hours $MIN minutes and $SEC seconds."

variables
