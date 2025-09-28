#include "../include/MidiSoundMapper.hpp"


MidiSoundMapper::MidiSoundMapper(){
    
    try {
        midiIn = new RtMidiIn();
    }
    catch ( RtMidiError &error ) {
        std::cerr << "ERROR IN CONSTRUCTOR\n";
        error.printMessage();
        exit( EXIT_FAILURE );
    }
};

MidiSoundMapper::~MidiSoundMapper(){
     delete midiIn;
}


int MidiSoundMapper::displayAvailablePorts()
{
    // Check inputs
    unsigned int nPorts = midiIn->getPortCount();

    cout << "There are " << nPorts << " MIDI input sources available.\n";
    std::string portName;
    bool isError = false;
    for(
        int i=0; 
        i < nPorts && isError == false; 
        i++ 
    ) {
        
        try 
        {
        portName = midiIn->getPortName(i);
        std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
        }
        
        catch ( RtMidiError &error ) 
        {
        error.printMessage();
        isError = true;
        }
        
    }
    
    return nPorts;
}


void MidiSoundMapper::ChoosePortInteractively(){
    
    int availablePorts = displayAvailablePorts();

    if(availablePorts == 0){
        cout << "Can't open any ports. Please, connect MIDI input device and try again.";
        return;
    }

    int portId;
    
    cout << "\nChoose a MIDI input port: ";
    std::cin >> portId;
    cout << "\n";

    try{
        midiIn->openPort(portId-1);
    }
    catch(RtMidiError& error){
        // error.printMessage();
        std::cerr << "Incorrect port id!\n";
        exit(EXIT_FAILURE);
    }
   
    return;
}
