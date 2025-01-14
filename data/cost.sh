#!/bin/sh

if [ $# -lt 4 ]; then
    echo "ERROR -- expects <nitems> <noptions> <optsize> <optfile>"
    exit 1
fi

file=cost.$(( $3 + 1 )).log
touch $file

count=1
while [ "$count" -le 1000 ]; do
    ../build/src/feasible_coverings_cost $1 $2 $3 < $4 1>> $file 2>&1
    sleep 1s
    count=$(( $count + 1 ))
done
