#!/bin/bash

# Path to your raylib source or compiled library
PROJECT_HEADERS="include/"
RTMIDI_HEADER="lib/rtmidi/"
RAYLIB_HEADER="lib/raylib/"
RAYLIB_LIB="lib/raylib/"

# Source file
CPP="src/main.cpp src/App.cpp src/Simulation.cpp src/Cell.cpp src/MidiReader.cpp lib/rtmidi/RtMidi.cpp"

# Output executable
OUT="game of life.exe"

# Compile with g++
g++ $CPP -o "$OUT" -I "$PROJECT_HEADERS" -I "$RAYLIB_HEADER" -I "$RTMIDI_HEADER" -L "$RAYLIB_LIB" -lraylib -lopengl32 -lgdi32 -lwinmm -D__WINDOWS_MM__

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Compilation succeeded: $OUT"
else
    echo "Compilation failed."
fi
