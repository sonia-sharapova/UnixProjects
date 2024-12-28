#!/bin/bash

# Compile msh code
gcc -I../include/ -o ../bin/msh src/*.c

# Check if compilation was successful
if [ $? -eq 0 ]; then
    # Run msh executable
    ../bin/msh
else
    echo "Compilation failed."
fi


