#include "../include/Cell.hpp"


uint8_t Cell::crossoverBits(uint8_t a, uint8_t b){
    // Choose splicing place
    uint8_t splittingPoint = rand()%8;
    
    uint8_t rightMask, leftMask;
    rightMask = (1u << splittingPoint) - 1;
    leftMask = ~rightMask;

    // Left
    uint8_t bLeftPart = b&leftMask;
    uint8_t aLeftPart = a&leftMask;
    
    // Right
    uint8_t aRightPart = a&rightMask;
    uint8_t bRightPart = b&rightMask;

    uint8_t c;

   
    if (rand()%2)
        c = bLeftPart + aRightPart;
    else 
        c = aLeftPart + bRightPart;

    if(rand()%1000 == 0)
        c = c^(1u << rand()%7+1); // mutate by inverting bit    

    return c;
}

Cell::Cell(int y, int x, CellType t, array<uint8_t, 4> rgba): 
    x(x), y(y), type(t)
{
    this->colorRGBA = rgba;
    this->colorMix = colorRGBA;
    this->counter = 0;
}


void Cell::SetColor(array<uint8_t, 4> rgba){
    this->colorRGBA = rgba;
    this->colorMix = rgba;
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
    return colorRGBA;
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
        // this->colorRGBA = {0,255,0,255};
        this->colorRGBA = this->colorMix; // Only Producers can be of different colors
        break;
    case REDUCER:
        this->colorRGBA = {255,0,0,255};
        break;
    default:
        break;    
    }
    
    this->colorMix = this->colorRGBA;
    counter = 0;
    return;
}

void Cell::ReceiveImpulse(int impulse, const array<uint8_t, 4>& receivedColor){
    counter += impulse;

    if((this->colorMix[0] + this->colorMix[1] + this->colorMix[2]) < 1)
        colorMix = receivedColor;
    else   
        for (size_t i = 0; i < 3; i++) // Doesn't evaluate A   
            this->colorMix[i] = 
                crossoverBits(this->colorMix[i], receivedColor[i]);
        
    return;
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
            
        tmpCell->ReceiveImpulse(impulseStrength, this->colorRGBA);
    }

    for (int i = 0; i < 2; i++){
        affectedY = (affectedY+1) % sizeY;
        tmpCell = &world[affectedY][affectedX];
        
        if(tmpCell->GetCounter() == 0 && tmpCell->IsAlive() == false)
            affectedCells.push_back(tmpCell);
            
        tmpCell->ReceiveImpulse(impulseStrength, this->colorRGBA);
    }


    for (int i = 0; i < 2; i++){
        affectedX = (affectedX-1+sizeX) % sizeX;
        tmpCell = &world[affectedY][affectedX];
        
        if(tmpCell->GetCounter() == 0 && tmpCell->IsAlive() == false)
            affectedCells.push_back(tmpCell);
            
        tmpCell->ReceiveImpulse(impulseStrength, this->colorRGBA);
    }


    for (int i = 0; i < 2; i++){
        affectedY = (affectedY-1+sizeY) % sizeY;
        tmpCell = &world[affectedY][affectedX];
        
        if(tmpCell->GetCounter() == 0 && tmpCell->IsAlive() == false)
            affectedCells.push_back(tmpCell);
            
        tmpCell->ReceiveImpulse(impulseStrength, this->colorRGBA);
    }


    return affectedCells;
}   