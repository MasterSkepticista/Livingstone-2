#!bash

#
rm -f 4-cyclic-failures.out
rm -f 4-cyclic-failures.scr
rm -f tmp-in run.tmp
touch tmp-in
for time in 1 2 3 4 ;
   do cat ../ispp-fail-outlet.scr  >> tmp-in;
done
cat tmp-in ../../exit.scr >> 4-cyclic-failures.scr
test_debug ../ispp-frame cover 200 < 4-cyclic-failures.scr  > run.tmp
grep -v ":0" run.tmp > 4-cyclic-failures.out
rm -f tmp-in run.tmp

