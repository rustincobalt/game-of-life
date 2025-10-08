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


// variable for keyboard interrupt
// atomic is for safe asynchronous access 
std::atomic<bool> isRunning(true); 

void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received.\n";
    isRunning = false;  // Tell main loop to stop
}


void MidiSoundMapper::ListenToMidi(){
    if(this->midiIn->isPortOpen() == false)
        throw "No port has been opened. Open MIDI Input port before listening.";
    
    vector<unsigned char> message;
    int nBytes, i;
    double stamp;

    midiIn->ignoreTypes(false, false, false);

    cout << "Reading MIDI from port "<< midiIn->getPortName() << ". Quit with Ctrl+C.\n";

    std::signal(SIGINT, signalHandler);

    std::bitset<sizeof(char)*8> binary;
    unsigned char messageValue;

    while(isRunning){

        stamp = midiIn->getMessage(&message);
        nBytes = message.size();


        // cout << nBytes << "\n";
        bool isPrinted = true;

        for ( i = 0; i < nBytes; i++)
        {   
            messageValue = message[i];
            if (messageValue == 0xFE || messageValue == 0xF8) {
                isPrinted = false;
                continue; // skip this iteration
                
                // 0xFE -> 1111 1110
                // 0xF2 -> 1111 1000
            }

            binary = messageValue;
            // cout << "Byte " << i << " = " << binary;
            cout << "Byte " << i << " = " << (int)messageValue << ", ";

            // if (i+1 != nBytes)
            //      cout << ", ";
            // else
            //     cout << '\n';
        }
        if ( isPrinted && nBytes > 0 )
            std::cout << "stamp = " << stamp << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }
    
    
    return;
}
