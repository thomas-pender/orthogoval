#!/bin/sh

gap -q <<EOF
for g in GeneratorsOfGroup(SP($1,2)) do
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
