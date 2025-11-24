#pragma once
#include "RtMidi.h"

#include <atomic>
#include <csignal>

#include <chrono>
#include <thread>

#include <vector>

#include <iostream>

using std::cout;
using std::vector;

/*Reads MIDI signals and stores active signals*/
class MidiReader {
    private:

    RtMidiIn* midiIn;
    
    int displayAvailablePorts();
    
    public:

    MidiReader();

    void ChoosePortInteractively();
    void ListenToMidi();

    ~MidiReader();
};
