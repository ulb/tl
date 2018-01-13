d="$1"
n="$(wc -l < "db/src/$d")"

slots=200
interval=5
jobs="hpc/jobs/$d"
queue="hpc/queue/$d"
running="hpc/running/$d"
done="hpc/done/$d"
out="hpc/out/$d"
log="hpc/log/$d"
resume="hpc/resume"

if [ ! -e "$resume" ] ; then

	# assumes no other manager is running
	dangling="$(squeue -h -u "$USER" -o "%j %i" | grep -E "^tl-$d-[1-9][0-9]*" | cut -d' ' -f2)"
	if [ "$dangling" != '' ] ; then scancel $dangling ; fi
	rm -rf hpc

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
	sed -e "s/#d/$d/g" -e "s/#base/$base/g" job.sh > "$job"
	>&2 echo "tl-$d-$base submission"
	sbatch "$job" && touch "$running/$base" && rm "$queue/$base"
}


while true ; do

	finished="$(count "$done")"

	if [ "$finished" -ge "$n" ] ; then break ; fi

	>&2 echo "$finished/$n jobs finished"

	for base in $(list "$running"); do
		if [ -e "$done/$base" ] ; then
			>&2 echo "tl-$d-$base done"
			rm "$running/$base"
		fi
	done

	nrunning="$(count "$running")"
	>&2 echo "$nrunning jobs running"

	available="$(($slots-$nrunning))"
	>&2 echo "$available slots available"

	started=0
	for base in $(list "$queue") ; do
		if [ "$started" -ge "$available" ] ; then break ; fi
		submit "$base"
		started="$(($started+1))"
	done

	>&2 echo "sleeping $interval seconds"
	sleep "$interval"

done

>&2 echo "All jobs finished"
