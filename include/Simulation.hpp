#pragma once

#include <../include/raylib.h>
#include <vector>
#include <random>
#include <utility> // for pair
#include <unordered_set>

using std::pair;
using std::vector;
using std::unordered_set;

// Custom hash for unordered_set

namespace std {
    template <>
    struct hash<pair<int, int>> {
        size_t operator()(const pair<int, int>& key) const {
            return hash<int>()(key.first) ^ hash<int>()(key.second);
        }  
    };
}


class Simulation{
    private:
    
    int rows;
    int cols;
    vector<vector<uint16_t>> world;
    vector<pair<int, int>> generation; 
    unordered_set<pair<int, int>> generationalChanges; // keeps track of affected cells + alive cells

    float cellSize;


    void drawCell(int, int);
    vector<pair<int, int>> sendImpulse(int, int);



    public:
    
    Simulation();
    Simulation(int rows, int cols, float cellSize);

    void InitRandom();
    void DrawAliveCells();
    void Update();
};