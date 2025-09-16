#pragma once

#include <raylib.h>
#include <Cell.hpp>

#include <utility>

#include <vector>
#include <random>
#include <array>

using std::array;
using std::vector;


class Simulation{
    private:

    int sizeY;
    int sizeX;
    float pixelSize;
    
    vector<vector<Cell>> world;

    vector<Cell*> generation; 
    vector<Cell*> generationalChanges;

    void drawCell(int, int, array<uint8_t, 4>);

    public:
    
    Simulation();
    Simulation(int sizeY, int sizeX, float pixelSize);

    void InitRandom();
    void DrawAliveCells();
    void Update();
};