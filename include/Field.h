#pragma once

#include <../include/raylib.h>
#include <vector>
#include <random>
#include <math.h>


using std::vector;



class Field{
    private:
    
    int rows;
    int cols;
    float cellSize;

    vector<vector<int>> cells;

    void drawCell(bool, int, int);

    public:
    Field();
    Field(int rows, int cols, float cellSize);

    void initRandom();
    void draw();
};