#include "../include/SimulationOld.hpp"


Texture2D Simulation::aliveCelltexture;
Texture2D Simulation::deadCelltexture;
char const Simulation::offsets[8][2] ={
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},          {0, 1},
        {1, -1},  {1, 0},  {1, 1}
};

Simulation::Simulation(int rows, int cols, float cellSize): 
    rows(rows),
    cols(cols),
    cellSize(cellSize),
    world(vector<vector<uint16_t>>(rows, vector<uint16_t>(cols, 0)))
{};

void Simulation::InitRandom(){
    int nCellsToInitiate = 8'000'000;

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
        col*cellSize,
        row*cellSize,
        cellSize,
        cellSize
    };

    if (world[row][col] / 100 > 0)
        //DrawRectangle(col*cellSize, row*cellSize, cellSize, cellSize, RED);
        //DrawTexture(Simulation::aliveCelltexture, col*cellSize, row*cellSize, ORANGE);
        DrawRectangleRec(rec, GREEN);
    else
        //DrawRectangle(col*cellSize, row*cellSize, cellSize, cellSize, BLACK);
        //DrawTexture(Simulation::deadCelltexture, col*cellSize, row*cellSize, BLACK);
        DrawRectangleRec(rec, BLACK);
    return;
}


void Simulation::sendImpulse(int row, int col){
    uint16_t* cellInfo;
    int neighborRow, neighborCol;
    int impulsesReceived, cellStatus;

    for (int i = 0; i < 8; i++)
    {      
        neighborRow = row + offsets[i][0];
        neighborCol = col + offsets[i][1];

        if(neighborRow < 0)
            neighborRow += rows;
        else if (neighborRow >= rows)
            neighborRow -= rows;

        if(neighborCol < 0)
            neighborCol += cols;
        else if (neighborCol >= cols)
            neighborCol -= cols;


        cellInfo = &world[neighborRow][neighborCol];

        impulsesReceived = (*cellInfo < 100) ? *cellInfo : *cellInfo -100;
        cellStatus = (*cellInfo < 100) ? 0 : 1;

        if(impulsesReceived == 0 && cellStatus == 0)
            generationalChanges.push_back(pair<int, int>(neighborRow, neighborCol));
        *cellInfo = *cellInfo + 1;    
    }
    return;
}

void Simulation::sendImpulseOld (int row, int col){
  
    int affRow, affCol; // ids of affected cell
    // take left upper corner
    affRow = (row-1 + rows) % rows;
    affCol = (col-1 + cols) % cols;

    for (int i = 0; i < 2; i++){
        affRow = (affRow+1) % rows;
        if(world[affRow][affCol] % 100 == 0)
            generationalChangesOld.insert(pair<int, int>(affRow, affCol));
            
        world[affRow][affCol]++;
    }

    for (int i = 0; i < 2; i++){
        affCol = (affCol+1) % cols;
        if(world[affRow][affCol] % 100 == 0)
            generationalChangesOld.insert(pair<int, int>(affRow, affCol));
        world[affRow][affCol]++;
        
    }
    
    for (int i = 0; i < 2; i++){
        affRow = (affRow-1 + rows) % rows;
        if(world[affRow][affCol] % 100 == 0)
            generationalChangesOld.insert(pair<int, int>(affRow, affCol));
        world[affRow][affCol]++;
        
    }

    for (int i = 0; i < 2; i++){
        affCol = (affCol-1 + cols) % cols;
        if(world[affRow][affCol] % 100 == 0)
            generationalChangesOld.insert(pair<int, int>(affRow, affCol));
        world[affRow][affCol]++;
        
    }

    return;
}






void Simulation::UpdateOld(){
   
    // Fill generationalChanges with alive cells (current generation)
    for (pair<int, int> &cell : generation)
        generationalChangesOld.insert(cell);

    // Fill generationalChanges with affected cells
    for (pair<int, int> &cell : generation)
        sendImpulse(cell.first, cell.second);
  
    generation.clear();


    uint16_t* cellInfo;
    int cellStatus, impulsesReceived;
    
    for (const pair<int, int> cell : generationalChangesOld)
    {
        cellInfo = &world[cell.first][cell.second];
        cellStatus = (*cellInfo < 100) ? 0 : 1;
        impulsesReceived = (*cellInfo < 100) ? *cellInfo : *cellInfo -100;

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
    
    generationalChangesOld.clear();
    return;
}

void Simulation::countAliveCells(){
    int aliveCount = 0;
    int deadCount = 0;
    int aliveImpulsed = 0;
    int deadImpulsed = 0;


    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (world[i][j] == 0)
                deadCount++;
            else if (world[i][j] < 100)
                deadImpulsed++;
            else if(world[i][j] == 100)
                aliveCount++;
            else
                aliveImpulsed++;
        }
        
    }

    cout << "Total Cells: " << rows*cols << "\n";
    cout << "Alive Cells: " << aliveCount << "\n";
    cout << "AlImp Cells: " << aliveImpulsed << "\n";
    cout << "Dead  Cells: " << deadCount<< "\n";
    cout << "DeImp Cells: " << deadImpulsed << "\n";
}

void Simulation::Update(){
    // Fill generationalChanges with alive cells (current generation)
    // cout << "Generation size BEFORE: " << generation.size() << "\n";
    pair<int, int> cell;
    for (int i = 0; i < generation.size(); i++)
    {
        cell = generation[i];
        generationalChanges.emplace_back(cell.first, cell.second);
    }
    

    // Fill generationalChanges with affected cells
    for (pair<int, int> &cell : generation)
        sendImpulse(cell.first, cell.second);
  
    generation.clear();


    uint16_t* cellInfo;
    int cellStatus, impulsesReceived;
    
    int aliveCount = 0;

    for (const pair<int, int> cell : generationalChanges)
    {
        cellInfo = &world[cell.first][cell.second];
        cellStatus = (*cellInfo < 100) ? 0 : 1;
        impulsesReceived = (*cellInfo < 100) ? *cellInfo : *cellInfo -100;
        if(cellStatus == 0 && impulsesReceived != 3)
            *cellInfo = 0;
       
        else if(cellStatus == 0 && impulsesReceived == 3){
            *cellInfo = 100;
            // drawCell(cell.first, cell.second);
            aliveCount++;
            generation.emplace_back(cell.first, cell.second);
        }
        else if (cellStatus > 0 && 
            (impulsesReceived == 2 || impulsesReceived == 3)
        ){
            *cellInfo = cellStatus * 100;
            aliveCount++;
            generation.emplace_back(cell.first, cell.second);
        }
        else if (cellStatus > 0 &&
            (impulsesReceived < 2 || impulsesReceived > 3)
        ){
            *cellInfo = 0;
            // drawCell(cell.first, cell.second);
        }
    }
    // cout << "Generational changes size: " << generationalChanges.size() << "\n";
    // cout << "Generation size AFTER: " << generation.size() << "\n";
    // cout << "Alive cells count added: " << aliveCount << "\n";

    // countAliveCells();

    generationalChanges.clear();
    return;
}