#include "..\lib\raylib\raylib.h"
#include "Simulation.hpp"


#include <string>

using std::string;

// #define TIMEDEBUG

#ifdef TIMEDEBUG
#include <chrono>
#include <thread>
#include <iostream>
#endif


class App {

private:
    int y;
    int x;
    float pixelSize;
    Color bgColor;
    int targetFPS;
    string windowName;


    void runAppLoop();

public:

    App(int , int , float , int = 120, string = "Game of Life",  Color = BLACK);
    void Launch();
    
};