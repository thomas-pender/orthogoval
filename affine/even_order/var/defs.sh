#!/bin/sh

# param 1 -- number of blocks
# param 2 -- size of each block
# param 3 -- exponent in q=p^n

defshead=../src/interface/defs.h
echo "# ifndef DEFS_H\n# define DEFS_H\n" > $defshead
echo "# define NB $1" >> $defshead
echo "# define LB $2" >> $defshead
echo "# define N $(expr $3 + $3)" >> $defshead
echo "# define NGENS 3\n" >> $defshead
echo "# endif" >> $defshead
