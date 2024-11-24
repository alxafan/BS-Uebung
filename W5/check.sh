#!/bin/sh

while IFS= read -r line; do
	users="`echo "$line" | cut -d: -f4 -s | tr ',' '\n'`"

	for user in $users; do
		if [ -z "`grep "^$user:" /etc/passwd`" ]; then
		       echo "$user $line"
		fi
	done	
done
