#include "../include/Simulation.hpp"
#include "Simulation.hpp"


Simulation::Simulation(int rows, int cols, float cellSize): 
    rows(rows),
    cols(cols),
    cellSize(cellSize),
    world(vector<vector<uint16_t>>(rows, vector<uint16_t>(cols, 0)))
{};

void Simulation::InitRandom(){
    int nCellsToInitiate = 20'000;

    int rowRandom, colRandom;

    while (nCellsToInitiate > 0)
    {
        rowRandom = std::rand() % rows;
        colRandom = std::rand() % cols;

        if (world[rowRandom][colRandom] == 0)
        {
            world[rowRandom][colRandom] = 100;
            generation.emplace_back(rowRandom, colRandom);
            nCellsToInitiate--;
        }
    }

    return;
}

void Simulation::DrawAliveCells(){
    for (pair<int, int> &cell : generation)
        drawCell(cell.first, cell.second);   
    return;
}

void Simulation::drawCell(int row, int col){

    Rectangle rec = {
        row*cellSize,
        col*cellSize,
        cellSize,
        cellSize
    };

    if (world[row][col] / 100 > 0)
        DrawRectangleRec(rec, GREEN);
    else
        DrawRectangleRec(rec, BLACK);

    return;
}


vector<pair<int, int>> Simulation::sendImpulse (int row, int col){
    vector<pair<int, int>> affectedCells;
    
    
    int affRow, affCol; // ids of affected cell
    // take left upper corner
    affRow = (row-1 + rows) % rows;
    affCol = (col-1 + cols) % cols;

    for (int i = 0; i < 2; i++){
        affRow = (affRow+1) % rows;
        world[affRow][affCol]++;
        affectedCells.emplace_back(affRow, affCol);
    }

    for (int i = 0; i < 2; i++){
        affCol = (affCol+1) % cols;
        world[affRow][affCol]++;
        affectedCells.emplace_back(affRow, affCol);
    }
    
    for (int i = 0; i < 2; i++){
        affRow = (affRow-1 + rows) % rows;
        world[affRow][affCol]++;
        affectedCells.emplace_back(affRow, affCol);
    }

    for (int i = 0; i < 2; i++){
        affCol = (affCol-1 + cols) % cols;
        world[affRow][affCol]++;
        affectedCells.emplace_back(affRow, affCol);
    }

    return affectedCells;
}






void Simulation::Update(){
   
    // Fill generationalChanges with alive cells (current generation)
    for (pair<int, int> &cell : generation)
        generationalChanges.insert(cell);

    // Fill generationalChanges with affected cells
    vector<pair<int, int>> affectedCells;
    for (pair<int, int> &cell : generation){
        affectedCells = sendImpulse(cell.first, cell.second);

        for (pair<int, int> &affCell : affectedCells)
            generationalChanges.insert(affCell);
        
    }
    
    generation.clear();


    uint16_t* cellInfo;
    int cellStatus, impulsesReceived;
    
    for (const pair<int, int> cell : generationalChanges)
    {
        cellInfo = &world[cell.first][cell.second];
        cellStatus = *cellInfo / 100;
        impulsesReceived = *cellInfo % 100;

        if(cellStatus == 0 && impulsesReceived != 3)
            *cellInfo = 0;
        
        else if(cellStatus == 0 && impulsesReceived == 3){
            *cellInfo = 100;
            // drawCell(cell.first, cell.second);
            generation.emplace_back(cell.first, cell.second);
        }
        else if (cellStatus > 0 && 
            (impulsesReceived == 2 || impulsesReceived == 3)
        ){
            *cellInfo = cellStatus * 100;
            generation.emplace_back(cell.first, cell.second);
        }
        else if (cellStatus > 0 &&
            (impulsesReceived < 2 || impulsesReceived > 3)
        ){
            *cellInfo = 0;
            // drawCell(cell.first, cell.second);
        }
    }
    
    generationalChanges.clear();
    return;
}