#!/bin/sh

echo_files() {
	echo "These are the current tests"
	for file in "$@"; do
		echo "-- $file"
	done
	echo "Usage: ./test.sh ##"
}

dir="./tests"

if [ $# -eq 0 ]; then
	echo_files $(ls $dir | grep -E '^[0-9]{2}.*\.bk$' | sort -V)

	exit 0
fi

arg=$(printf "%02d" $1)
filename=$(ls $dir | grep -E "^$arg.*" | sed -n '/\.bk$/p' | sed "s/\.bk$//")
filepath="${dir}/${filename}.bk"

if [ -z "$filename" ]; then
	echo "No file found with prefix $arg"
	exit 1
fi

echo "Running test: $filename"
./run.sh --silent-build --clean --clear --file "$filepath"
exit_code=$?

rm -f "${dir}/${filename}.asm" "${dir}/${filename}.o" "${dir}/${filename}"

if [ $exit_code -eq 0 ]; then
    echo "TEST SUCCESSFUL!"
else
    echo "TEST FAILED!"
    exit $exit_code
fi
