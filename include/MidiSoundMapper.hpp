#pragma once
#include "ISoundMapper.hpp"
#include "RtMidi.h"

#include <iostream>
#include <cstdlib>
// #include <signal.h> // For...?

#include <atomic>
#include <csignal>

#include <vector>

#include <chrono>
#include <thread>

#include <bitset>

using std::cout;
using std::vector;
/*
Class requirements:
1. Initialization 
2. Reading the input
3. Mapping
*/


class MidiSoundMapper : ISoundMapper {
    private:
    RtMidiIn* midiIn;

    
    int displayAvailablePorts();
    
    public:

    MidiSoundMapper();

    void ChoosePortInteractively();
    void ListenToMidi();

    void MapSoundToCell(){};
    ~MidiSoundMapper();

};