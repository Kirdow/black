#!/bin/sh

execute_command() {
	local number=$1

	./test.sh $number --no-clean >/dev/null 2>&1

	if [ $? -eq 0 ]; then
		echo "$number SUCCESS"
	else
		echo "$number FAILED"
	fi
}

if [ -z "$1" ]; then
	echo "Please provide an upper bound."
	exit 1
fi

for number in $(seq 0 $1); do
	execute_command $number &
done

wait
