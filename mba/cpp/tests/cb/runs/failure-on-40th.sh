#!bash

#
rm -f failure-on-40th.out
rm -f failure-on-40th.scr
rm -f tmp-in run.tmp
touch tmp-in
for time in 1 2 3 4 5 6 7 8 9 10 11 12 13 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 ;
   do cat ../cycle-7-8-nofail.scr >> tmp-in;
done
cat ../cycle-7-8.scr >> tmp-in;

for time in 1 2 3 4 5 6 7 8 9 10 ;
   do cat ../cycle-7-8-nofail.scr >> tmp-in;
done

cat tmp-in ../../exit.scr >> failure-on-40th.scr
test_debug ../cb cover 200 < failure-on-40th.scr  > run.tmp
grep -v ":0" run.tmp >  failure-on-40th.out
rm -f tmp-in run.tmp