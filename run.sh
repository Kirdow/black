#!/bin/sh

# flags
is_clean=false
is_clear=false

filename=test.bk
next_file=false

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
        --file)
            next_file=true
            ;;
        *)
            if $next_file; then
                filename="$arg"
                next_file=false
            else
                argv="$argv $arg"
            fi
            ;;
    esac
done

# Check if arguments contains "clean"
if $is_clean; then
    if ! $is_clear; then
        # Print cleanup step
        echo Cleanup before build
    fi

    # Cleanup here
    rm -rf build
fi

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
./build/black $filename $argv
exit $?
