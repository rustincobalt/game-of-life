#include "../include/Simulation.hpp"
#include "../include/Cell.hpp"

Simulation::Simulation(int sizeY,int sizeX, float pixelSize): 
    sizeY(sizeY),
    sizeX(sizeX),
    pixelSize(pixelSize){
        vector<Cell> tmpVec;
        
        for (size_t i = 0; i < sizeY; i++)
        {   
            
            for (size_t j = 0; j < sizeX; j++)
                tmpVec.push_back(
                    Cell(i, j, DEAD, array<uint8_t, 4>{0, 0, 0, 255} )
                );
            
            world.push_back(tmpVec);
            tmpVec.clear();
        }
        
    };

void Simulation::InitRandom(){
    int nCellsToInitiate = 200'000;

    int yRandom, xRandom;
    CellType type;
    array<uint8_t, 4> rgba;
    while (nCellsToInitiate > 0)
    {
        yRandom = std::rand() % this->sizeY;
        xRandom = std::rand() % this->sizeX;
        int half = (sizeY*sizeX)/2;
        if (world[yRandom][xRandom].IsAlive() == false)
        {   
            // type = ((yRandom*sizeX + xRandom) > half) ? PRODUCER : REDUCER;
            type = ((std::pow(yRandom - sizeY/2, 2) + std::pow(xRandom-sizeX/2, 2)) < std::pow((sizeX+sizeY)/8, 2)) ? PRODUCER : REDUCER;
            // type = PRODUCER;
            (type == PRODUCER) ? rgba = {0,255,0,255} : rgba = {255,0,0,255};
            world[yRandom][xRandom].SetType(type);
            world[yRandom][xRandom].SetColor(rgba);
            generation.push_back(&world[yRandom][xRandom]);
            nCellsToInitiate--;
        }
    }

    return;
}

void Simulation::DrawAliveCells(){
    for (const Cell* cell : generation)
        drawCell(cell->GetY(), cell->GetX(), cell->GetColor());   
    return;
}

void Simulation::drawCell(int y, int x, array<uint8_t, 4> colorRGBA){

    Rectangle rec = {
        x*pixelSize,
        y*pixelSize,
        pixelSize,
        pixelSize
    };
    
    Color color = {
        static_cast<unsigned char>(colorRGBA[0]),
        static_cast<unsigned char>(colorRGBA[1]),
        static_cast<unsigned char>(colorRGBA[2]),
        static_cast<unsigned char>(colorRGBA[3])
    };

    DrawRectangleRec(rec, color);
    return;
}


void Simulation::Update(){
    // Fill generationalChanges with alive cells (current generation)
    // cout << "Generation size BEFORE: " << generation.size() << "\n";

    generationalChanges = generation;
    
    vector<Cell*> affectedDeadCells;
    // Fill generationalChanges with affected cells
    for (Cell* cell : generation){
        affectedDeadCells = cell->SendImpulse(world, sizeY, sizeX);
        // generationalChanges.insert(generationalChanges.end(), affectedDeadCells.begin(), affectedDeadCells.end());

        for (auto &&i : affectedDeadCells)
        {
            generationalChanges.push_back(i);
        }
        
    }

    generation.clear();


    for(Cell* cell : generationalChanges){
        cell->EvaluateSelf();
        if(cell->IsAlive())
            generation.push_back(cell);
    }

    generationalChanges.clear();
    return;
}