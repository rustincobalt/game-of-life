#include "..\include\App.hpp"
#include "..\include\MidiReader.hpp"

int main(){
    // App GameOfLife = App(500, 800, 2, 120);
    // GameOfLife.Launch();
    
    MidiReader reader;
    reader.ChoosePortInteractively();
    reader.ListenToMidi();

    return 0;
}