#!/bin/sh

i=0
if [ $# -eq 0 ]; then
	while IFS= read -r line; do
		i=`expr $i + 1`
		if [ -n "`echo $line | cut -d: -f4`" ]; then
			echo "$i $line"
		fi		
	done
elif [ $# -eq 1 -a -f $1 ]; then
	while IFS= read -r line; do
		i=`expr $i + 1`
		if [ -n "`echo $line | cut -d: -f4`" ]; then 
			echo "$i $line"
		fi
	done < $1
else
	echo "extract: $1 is not a regular file or more than argument was given"
	exit 1
fi	
