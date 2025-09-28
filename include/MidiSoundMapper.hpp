#pragma once
#include "ISoundMapper.hpp"
#include "RtMidi.h"

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
using std::cout;

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

    void MapSoundToCell(){};
    ~MidiSoundMapper();

};