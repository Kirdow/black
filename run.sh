#!/bin/sh

# flags
is_clean=false
is_clear=false
is_silent=false
is_cat=false

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
			is_silent=true
            ;;
		--clear)
			is_clear=true
			;;
		--cat)
			is_cat=true
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
        printf "\e[32mCleanup before build\e[0m\n"
    fi

    # Cleanup here
    rm -rf build
fi

# Create build/ if not exist
if [ ! -d "build/" ]; then
    mkdir build 
fi

if ! $is_clear && ! $is_silent; then
	# Print build step
	printf "\e[32mBuilding black\e[0m\n"
fi

# Check if arguments contains "clear"
if $is_silent; then
    # Compile program
    cmake -S . -B build/ > /dev/null 2>&1

    # Build/Link program
    cmake --build build/ > /dev/null 2>&1
else
    # Compile program
    cmake -S . -B build/

    # Build/Link program
    cmake --build build/
fi

if $is_cat; then
	if ! $is_clear; then
		# Print cat notice
		printf "\e[32m$filename source:\e[0m\n"
	fi
	cat $filename
fi

if ! $is_clear; then
    # Print before running
    printf "\e[32mRUNNING black\e[0m\n"
fi

# Run the program
./build/black $filename $argv
exit $?
