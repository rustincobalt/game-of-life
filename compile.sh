#!/bin/bash

# Path to your raylib source or compiled library
RAYLIB_HEAD="include/"
RAYLIB_LIB="lib/"

# Source file
SRC="src/main.cpp"

# Output executable
OUT="game of life.exe"

# Compile with g++
g++ "$SRC" -o "$OUT" -I "$RAYLIB_HEAD" -L "$RAYLIB_LIB" -lraylib -lopengl32 -lgdi32 -lwinmm

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Compilation succeeded: $OUT"
else
    echo "Compilation failed."
fi
