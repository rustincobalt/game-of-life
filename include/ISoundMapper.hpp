#pragma once
class ISoundMapper{
    public:
    
    virtual void MapSoundToCell() = 0;
    virtual ~ISoundMapper(){};
};