#!/bin/sh

# flags
is_clean=false
is_clear=false

argv=
# Iterate over arguments
for arg in "$@"; do
    case "$arg" in
        --clean)
            is_clean=true
            ;;
        --silent-build)
            is_clear=true
            ;;
        *)
            argv="$argv $arg"
            ;;
    esac
done

# Check if arguments contains "clean"
if $is_clean; then
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

# Check if arguments contains "clear"
if $is_clear; then
    # Compile program
    cmake -S . -B build/ > /dev/null 2>&1

    # Build/Link program
    cmake --build build/ > /dev/null 2>&1

else
    # Compile program
    cmake -S . -B build/

    # Build/Link program
    cmake --build build/
    
    # Print before running
    echo
    echo RUNNING black
fi

# Run the program
./build/black test.bk $argv

