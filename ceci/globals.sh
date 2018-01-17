#!/usr/bin/env sh

bases="db/src/$d"
n="$(wc -l < "$bases")"
polytopes="db/src/$(($d+1))"
slots=200
interval=5
scratch="$GLOBALSCRATCH/tl/$d"
jobs="$scratch/jobs"
queue="$scratch/queue"
running="$scratch/running"
done="$scratch/done"
fail="$scratch/fail"
out="$scratch/out"
log="$scratch/log"
duration="$scratch/duration"
resume="$scratch/resume"
template_job="ceci/job.sh"
merged="$scratch/merged"

function now {
	date '+%Y-%m-%d_%H:%M:%S'
}

function info {
	>&2 echo "[$(now)]" "$@"
}

function variables {
	info "d=$d"
	info "bases=$bases"
	info "n=$n"
	info "polytopes=$polytopes"
	info "slots=$slots"
	info "interval=$interval"
	info "scratch=$scratch"
	info "jobs=$jobs"
	info "queue=$queue"
	info "running=$running"
	info "done=$done"
	info "fail=$fail"
	info "out=$out"
	info "log=$log"
	info "resume=$resume"
	info "template_job=$template_job"
	info "merged=$merged"
}
