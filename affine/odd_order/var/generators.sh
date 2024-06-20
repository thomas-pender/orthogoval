#!/bin/sh

# param 1 -- degree
# param 2 -- order of prime field

gap -q <<EOF
for g in GeneratorsOfGroup(GL($1,$2)) do
    for i in [1..$1] do
    for j in [1..$1] do
        Print(Int(g[i,j])," ");
    od;
      Print("\n");
    od;
    Print("\n");
od;

quit;
EOF
