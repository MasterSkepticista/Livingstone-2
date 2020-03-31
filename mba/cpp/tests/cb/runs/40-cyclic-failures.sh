#!bash

#
rm -f 40-cyclic-failures.out
rm -f 40-cyclic-failures.scr
rm -f tmp-in run.tmp
touch tmp-in
for time in 1 2 3 4 5 6 7 8 9 10 11 12 13 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40;
   do cat ../cycle-7-8.scr >> tmp-in;
done
cat tmp-in ../../exit.scr >> 40-cyclic-failures.scr
test_debug ../cb cover 200 < 40-cyclic-failures.scr  > run.tmp
grep -v ":0" run.tmp > 40-cyclic-failures.out
rm -f tmp-in run.tmp

