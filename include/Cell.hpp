#pragma once
#include <raylib.h>

#include <vector>
#include <utility>
#include <array> // return array char
#include <cstdint>
#include <math.h>

using std::vector;
using std::pair;
using std::array;


enum CellType {
    DEAD,
    PRODUCER,
    REDUCER
};

class Cell{
    private:

    int y;
    int x;
    int counter;
    CellType type;

    array<uint8_t, 4> colorRGBA; // size 4
  

    public:
    Cell(int y, int x, CellType t, array<uint8_t, 4> rgba);
    ~Cell() = default;

    // Setter
    void SetColor(array<uint8_t, 4>);
    void SetType(CellType);

    // Getter
    int GetY() const;
    int GetX() const;
    int GetCounter() const;
    array<uint8_t, 4> GetColor() const;

    // Functionality
    bool IsAlive();
    vector<Cell*> SendImpulse(vector<vector<Cell>>& world, int sizeY, int sizeX);
    void ReceiveImpulse(int);
    void EvaluateSelf();
};