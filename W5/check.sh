#!/bin/sh

if test $# -eq 0; then
	while read -r line; do
		for users in "`echo $line | cut -d: -f4`"; do
			for user in ${users//,/ }; do
				if ! cat /etc/passwd | grep "$user" > /dev/null ; then
					echo "$user $line " 
				fi
			done
		done
	done
else
	if test $# -eq 1 -a -f $1; then
		while read -r line; do
			for users in "`echo $line | cut -d: -f4`"; do
				for user in ${users//,/ }; do
					if ! cat /etc/passwd | grep "$user" > /dev/null ; then
						 echo "$user $line " 
					fi
				done
			done
		done < $1
	else
		echo "n1: $1 is not a regular file or more than one argument was given"
		exit 1
	fi
fi
exit 0

