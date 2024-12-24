#!/bin/sh

gap -q ${1} <<EOF
G:=Group(GP);;
for g in G do
    for i in [1..Length(g)] do
    for j in [1..Length(g)] do
        Print(Int(g[i,j]), " ");
    od;
        Print("\n");
    od;
    Print("\n");
od;
EOF
