#!/bin/bash

# Get list of libraries
LIBS=$(i686-w64-mingw32-pkg-config --libs --static gtk+-3.0)

# Loop over libraries and check if they exist and are statically compilable
for LIB in $LIBS; do
    if [[ $LIB != -l* ]]; then
        continue
    fi
    LIBFILE="/mingw32/lib/lib${LIB:2}.a"
    if [[ ! -f $LIBFILE ]]; then
        echo "$LIBFILE is not found"
        continue
    fi
    if ! i686-w64-mingw32-g++ -static -o /dev/null -xc++ - "$LIBFILE" >&/dev/null; then
        echo "$LIBFILE is not statically compilable"
        continue
    fi
done

