d="$1"

slots=20
interval=30
jobs="hpc/jobs/$d"
running="hpc/running/$d"
done="hpc/done/$d"
out="hpc/out/$d"
log="hpc/log/$d"

mkdir -p "$jobs"
mkdir -p "$running"
mkdir -p "$done"
mkdir -p "$out"
mkdir -p "$log"

function submit {
	base="$1"
	job="$jobs/$base.sh"
	sed -e "s/#d/$d/g" -e "s/#base/$base/g" job.sh > "$job"
	>&2 echo "tl-$d-$base submission"
	sbatch "$job" && touch "$running/$base"
}

i=1
n="$(wc -l < db/src/$d)"

while [ "$i" -le "$n" ] ; do

	for base in $(find "$running" -type f -printf "%f\n"); do
		if [ -e "$done/$base" ] ; then
			>&2 echo "tl-$d-$base done"
			rm "$running/$base"
		fi
	done

	nrunning="$(find "$running" -type f -printf "%f\n" | wc -l)"
	>&2 echo "$nrunning jobs running"

	available="$(($slots-$nrunning))"
	>&2 echo "$available slots available"

	for ((j=0;j<$available;j++)); do
		base="$(($i+$j))"
		if [ "$base" -gt "$n" ] ; break ; fi
		submit "$base"
	done

	i="$(($i+$available))"

	>&2 echo "sleeping $interval seconds"
	sleep "$interval"

done
