#!/bin/sh

sage -q 1> /dev/null 2>&1 <<EOF
f=open("${1}","r");
A=[matrix([[int(x) for x in y.split()] for y in z.split('\n')]) for z in f.read().split('\n\n')[:-1]];
f=open("${1}","w");
f.write("GP:=[\n"+",\n".join(z for z in ["\t[\n"+",\n".join(y for y in ["\t["+",".join("ZmodnZObj(%d,2)" % x for x in row)+"]" for row in a.rows()])+"\n\t]" for a in A])+"\n];\n");
f.close();
quit;
EOF

sed -i 's/$/ \\/' ${1}
