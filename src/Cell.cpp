#include "../include/Cell.hpp"

Cell::Cell(int y, int x, CellType t, array<uint8_t, 4> rgba): 
    x(x), y(y), type(t)
{
    for (int i = 0; i < 4; i++)
        colorRGBA[i] = rgba[i];
    counter = 0;
}


void Cell::SetColor(array<uint8_t, 4> rgba){
    this->colorRGBA = rgba;
}

void Cell::SetType(CellType t){
    this->type = t;
}

int Cell::GetY() const{
    return y;
}

int Cell::GetX() const{
    return x;
}

int Cell::GetCounter() const{
    return counter;
}

array<uint8_t, 4> Cell::GetColor() const{
    return {
        colorRGBA[0],
        colorRGBA[1],
        colorRGBA[2],
        colorRGBA[3]
    };
}

bool Cell::IsAlive(){
    return (type == DEAD) ? false : true;
}

void Cell::EvaluateSelf(){
    
    /*
    DEAD -> ALive if c = 3 or -3
    (... - -3) (-3; 3) (3 - ...) - DEADD
    {-3} - REDUCER
    {3} - PRODUCER


    PRODUCER
    (... - -3) (-2 2) (3 - ...) - DEAD
    [-3 -2] - REDUCER 
    [2 3] - PRODUCER


    REDUCER
    (... - -3) (-2 2) (3 - ...) - DEAD
    [-3 -2] - REDUCER 
    [2 3] - PRODUCER
    */

    int absCounter = std::abs(counter);
    int counterSign = (counter > 0) ? 1 :  -1;

    if(this->type == DEAD && absCounter == 3)
        this->type = (counter > 0) ? PRODUCER : REDUCER;
    else if (this->type != DEAD && 
        (absCounter < 2  || absCounter > 3))
            this->type = DEAD;
    else if(this->type == PRODUCER && counter < 0)
        this->type = REDUCER;
    else if(this->type == REDUCER && counter > 0)
        this->type = PRODUCER;


    switch(this->type){
    case DEAD:
        this->colorRGBA = {0,0,0,255};
        break;
    case PRODUCER:
        this->colorRGBA = {0,255,0,255};
        break;
    case REDUCER:
        this->colorRGBA = {255,0,0,255};
        break;
    default:
        break;    
    }
    
    counter = 0;
    return;
}

void Cell::ReceiveImpulse(int impulse){
    counter += impulse;
}

vector<Cell*> Cell::SendImpulse(vector<vector<Cell>>& world, int sizeY, int sizeX){
    
    int impulseStrength;
    if (this->type == PRODUCER)
        impulseStrength = 1;
    else if (this->type == REDUCER)
        impulseStrength = -1;

    // TODO: error
    

    vector<Cell*> affectedCells;


    int affectedY, affectedX; // ids of affected cell
    Cell* tmpCell;
    // take left upper corner
    affectedY = (y-1 + sizeY) % sizeY;
    affectedX = (x-1 + sizeX) % sizeX;
    
   

    for (int i = 0; i < 2; i++){
        affectedX = (affectedX+1) % sizeX;
        tmpCell = &world[affectedY][affectedX];
        
        if(tmpCell->GetCounter() == 0 && tmpCell->IsAlive() == false)
            affectedCells.push_back(tmpCell);
            
        tmpCell->ReceiveImpulse(impulseStrength);
    }

    for (int i = 0; i < 2; i++){
        affectedY = (affectedY+1) % sizeY;
        tmpCell = &world[affectedY][affectedX];
        
        if(tmpCell->GetCounter() == 0 && tmpCell->IsAlive() == false)
            affectedCells.push_back(tmpCell);
            
        tmpCell->ReceiveImpulse(impulseStrength);
    }


    for (int i = 0; i < 2; i++){
        affectedX = (affectedX-1+sizeX) % sizeX;
        tmpCell = &world[affectedY][affectedX];
        
        if(tmpCell->GetCounter() == 0 && tmpCell->IsAlive() == false)
            affectedCells.push_back(tmpCell);
            
        tmpCell->ReceiveImpulse(impulseStrength);
    }


    for (int i = 0; i < 2; i++){
        affectedY = (affectedY-1+sizeY) % sizeY;
        tmpCell = &world[affectedY][affectedX];
        
        if(tmpCell->GetCounter() == 0 && tmpCell->IsAlive() == false)
            affectedCells.push_back(tmpCell);
            
        tmpCell->ReceiveImpulse(impulseStrength);
    }


    return affectedCells;
}   