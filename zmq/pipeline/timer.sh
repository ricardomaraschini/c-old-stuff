#!/bin/sh

STOPAT=47478
DONTSTOP=1

while [ $DONTSTOP -ne 0 ];
do
	RET=`./stats | egrep ^d |cut -f2`
	if [ -z $RET ];
	then
		sleep 0.5
		continue
	fi

	if [ $RET -eq $STOPAT ];
	then
		DONTSTOP=0
	else
		sleep 0.5
	fi
done

