#!/bin/bash

packet_count=500000
payload_size=742
benchmark_runs=3
tests_count=3

tests=( $@ )

pcap_file=$(mktemp -t benchmark.XXXXXXXXXX)
#results_file=$(mktemp -t benchmark-results.XXXXXXXXXX)
results_file="results"

for test in $(seq 1 $tests_count)
do
    echo "[i] Generating $packet_count packets, each of size $packet_size"
    ./pcap_generator/generator $pcap_file $test $packet_count $payload_size
    echo "[+] Done"
    echo "[i] Executing test case $test"

    for i in ${tests[@]}
    do
        echo "[i] Executing benchmark for library $i"
        benchmark_input="packet"
        if [ $test -eq 3 ]
        then
            benchmark_input="dns"
        fi
        echo ./$i/benchmark $pcap_file $benchmark_input $benchmark_runs
        output=$(./$i/benchmark $pcap_file $benchmark_input $benchmark_runs )
        echo "$test $i $output" >> $results_file
    done
done

rm $pcap_file
echo "[+] Done. Results saved in $results_file"
