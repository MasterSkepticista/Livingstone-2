#!bash

#
rm -f 2-cyclic-failures.out
rm -f 2-cyclic-failures.scr
rm -f tmp-in run.tmp
touch tmp-in
for time in 1 2 ;
   do cat ../ispp-fail-outlet.scr  >> tmp-in;
done
cat tmp-in ../../exit.scr >> 2-cyclic-failures.scr
test_debug ../ispp-frame cover 200 < 2-cyclic-failures.scr  > run.tmp
grep -v ":0" run.tmp > 2-cyclic-failures.out
rm -f tmp-in run.tmp

