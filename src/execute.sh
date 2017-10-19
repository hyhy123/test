#!/bin/bash

stat=0;
ite=15;
ran=`date +%M%S`
MC=40000

fout=output/Stat_$MC\_$ran.out
echo $fout

for (( i=0;i<$ite;i++ ))
do
    res=`SHarmonic <<EOF
     	         1000
		 $MC
		 0.25
		 1
		 100
		 $i
EOF`
    echo -n -e "$i\t" >> $fout;
    corr=`echo $res | awk '{print $17}'`
    echo -n -e "$corr\t" >> $fout;
    err=`echo $res | awk '{print $19}'`
    echo "$err" >> $fout;
#    stat=`echo $stat+$tmp | bc`	
    sleep 3
    echo $i/$ite;
done

#echo "scale=5;$stat/$ite" | bc
