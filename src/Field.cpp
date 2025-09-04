#include "../include/Field.h"


Field::Field(int rows, int cols, float cellSize): 
    rows(rows),
    cols(cols),
    cellSize(cellSize),
    cells(vector<vector<int>>(rows, vector<int>(cols, 0)))
{};

void Field::initRandom(){
    int nCellsToInitiate = 100;

    int rowRandom, colRandom;

    while (nCellsToInitiate > 0)
    {
        rowRandom = std::rand() % rows;
        colRandom = std::rand() % cols;

        if (cells[rowRandom][colRandom] == 0)
        {
            cells[rowRandom][colRandom] = 1;
            nCellsToInitiate--;
        }
    }

    return;
}

void Field::draw(){
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (cells[i][j])
                drawCell(true, i, j);
            else
                drawCell(false, i, j);
            
        }
        
    }
    
    
}

void Field::drawCell(bool isAlive, int row, int col){

    Rectangle rec = {
        row*cellSize,
        col*cellSize,
        cellSize,
        cellSize
    };

    if (isAlive)
        DrawRectangleRec(rec, GREEN);
    else
        DrawRectangleRec(rec, BLACK);

    return;
}

