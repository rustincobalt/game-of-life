#pragma once

#include <../include/raylib.h>
#include <vector>
#include <random>
#include <utility> // for pair
#include <unordered_set>

#include <iostream>
using std::cout;

using std::pair;
using std::vector;
using std::unordered_set;

// 0x - HEXADECIMAL
// ULL - Unsigned Long Long
// U - Unsigned
#define HASH64CONST 0x9e3779b97f4a7c15ULL
#define HASH32CONST 0x9e3779b9U
// 11'400'714'819'323'198'485
// Custom hash for unordered_set
namespace std {
    template <>
    struct hash<pair<int, int>> {
        size_t operator()(const pair<int, int>& key) const {
            // return hash<int>()(key.first) ^ hash<int>()(key.second);
            size_t h1 = hash<int>()(key.first);
            size_t h2 = hash<int>()(key.second);

            size_t resultingHash =  h1^( h2 + HASH64CONST + (h1 << 6) + (h1 >>2));
            // size_t betterHash = hash<size_t>()(resultingHash);
            return resultingHash;
        }  
    };
}



class Simulation{
    private:

    int rows;
    int cols;
    vector<vector<uint16_t>> world;
    vector<pair<int, int>> generation; 
    unordered_set<pair<int, int>> generationalChangesOld; // keeps track of affected cells + alive cells
    vector<pair<int, int>> generationalChanges;

    float cellSize;
    static const char offsets[8][2];

    void drawCell(int, int);
    void sendImpulse(int, int);
    void sendImpulseOld(int, int);

    void countAliveCells();

    public:
    
    static Texture2D aliveCelltexture;
    static Texture2D deadCelltexture;

    Simulation();
    Simulation(int rows, int cols, float cellSize);

    void InitRandom();
    void DrawAliveCells();
    void UpdateOld();
    void Update();
};