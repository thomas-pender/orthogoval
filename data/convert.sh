#!/bin/sh

python3 <<EOF
f=open('$1','r')
mats=[[[int(x) for x in y.split()] for y in z.split('\n')] for z in f.read().split('\n\n')[:-1]]

f.close()
f=open('$1','w')

for mat in mats:
    for row in mat:
        t=f.write(str(sum(row[i]*(2**(len(row)-i-1)) for i in range(len(row))))+' ')
    t=f.write('\n')

quit;
EOF
