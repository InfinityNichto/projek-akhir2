#!/bin/bash
make && ./projek-akhir2

counter=$(<../build_counter)
((counter++))
printf "%s" "$counter" > ../build_counter
echo -n "build counter: "
cat ../build_counter
echo