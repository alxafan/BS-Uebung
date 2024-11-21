#!/bin/sh

i=0
if [ $# -eq 0 ]; then
	while read -r line; do
		i=`expr $i + 1`
		echo "$i $line"
	done
else
	if [ $# -eq 1 -a -f $1 ]; then
		while read -r line; do
			i=`expr $i + 1`
			echo "$i $line"  
		done < $1
	else
		echo "n1: $1 is not a regular file or more than one argument was given"
		exit 1
	fi
fi
exit 0
