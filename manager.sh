d="$1"

maxjobs=20
jobs="hpc/jobs/$d"
running="hpc/running/$d"
done="hpc/done/$d"
out="db/out/$d"

mkdir -p "$jobs"
mkdir -p "$running"
mkdir -p "$done"
mkdir -p "$out"

function submit {
	base="$1"
	job="jobs/$d/$base.sh"
	sed -e "s/#d/$d/g" -e "s/#base/$base/g" job.sh > "$job"
	sbatch "$job"
}

i=0
n="$(wc -l < db/src/$d)"

while true ; do

	for base in $(find "$running" -type f -printf "%f\n"); do
		if [ -e "$done/$base" ] ; then rm "$done/$running" fi
	done

	nrunning="$(find "$running" -type f -printf "%f\n" | wc -l)"

	available="$(($maxjobs-$nrunning))"

	for ((j=0;j<$available;j++)); do
		base="$(($i+$j))"
		submit "$base"
	done

	i="$(($i+$available))"

	sleep 30

done
