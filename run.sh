#!/bin/sh

# Check if argument contains "clean"
if [ "$1" = "clean" ]; then
    # Print cleanup step
    echo Cleanup before build

    # Cleanup here
    rm -rf build
fi

# Exit the script if anything fails
set -e

# Create build/ if not exist
if [ ! -d "build/" ]; then
    mkdir build 
fi

# Compile program
cmake -S . -B build/

# Build/Link program
cmake --build build/

# Print before running
echo
echo RUNNING black

# Run the program
./build/black test.bk

