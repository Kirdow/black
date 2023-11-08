#!/bin/sh

filename=$1

filepath=${filename}.bk

echo "Running test: $filename"
./run.sh --silent-build --clean --clear --file tests/$filepath
exit_code=$?

rm tests/${filename}.asm
rm tests/${filename}.o
rm tests/${filename}

if [ $exit_code -eq 0 ]; then
    echo "TEST SUCCESSFUL!"
else
    echo "TEST FAILED!"
    exit $exit_code
fi
