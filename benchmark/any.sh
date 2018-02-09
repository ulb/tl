#!/usr/bin/env bash

rep=20
inp='/dev/null'
out='benchmark/times/default'
cmd='echo 1'

run_tests() {
    # --------------------------------------------------------------------------
    # Benchmark loop
    # --------------------------------------------------------------------------
    echo "Benchmarking < \"$inp\" $cmd ...";

    # Run the given command [rep] times
    for (( i = 1; i <= $rep ; i++ ))
    do
        # percentage completion
        p=$(( $i * 100 / $rep))

        # runs time function for the called script, output in a comma seperated
        # format output file specified with -o command and -a specifies append
        START="$(date +%s%N)"
        < "$inp" $cmd > /dev/null 2>&1
        END="$(date +%s%N)"
        >> "$out" echo $(( $END - $START ))

        # Clear the HDD cache (I hope?)
        #sync && echo 3 > /proc/sys/vm/drop_caches

        echo -ne ${p}'% \r'
    done;

    echo -ne '\n'

}

# Option parsing
while getopts n:i:c:o: OPT
do
    case "$OPT" in
        n)
            rep=$OPTARG
            ;;
        i)
            inp=$OPTARG
            ;;
        o)
            out=$OPTARG
            ;;
        c)
            cmd=$OPTARG
            ;;
        \?)
            echo 'No arguments supplied'
            exit 1
            ;;
    esac
done

run_tests

shift `expr $OPTIND - 1`
