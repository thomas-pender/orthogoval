#!/bin/sh

if [ $# -eq 0 ]; then echo "Error -- expects exponent as parameter." ; exit 1 ; fi

sage -q 1> /dev/null 2>&1 <<EOF
n=$1
q=2**n
q2=q**2

mod=GF(q2).modulus()
FF.<x>=Zmod(2)[]
F=list(FF.quotient(mod,'x'))
g=F[2]

C=[[F[0]]+[g**(j*(q+1)+i) for j in range(q-1)] for i in range(q+1)]
D=[matrix(GF(2),[c.list() for c in CC]).row_space().basis() for CC in C]

f=open('spread_basis.%d.txt' % q,'w')
for basis in D:
    for d in basis:
        t=f.write('%d ' % (sum(int(d[k])*(2**k) for k in range(len(d)))))
    t=f.write('\n')
f.flush()
f.close()

quit;
EOF
