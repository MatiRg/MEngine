#pragma once
#include "Entity.hpp"

class CEngine;

class CWorld final: public CEntity
{
public:
    CWorld(CEngine*);
    ~CWorld();

    ENTITY(CWorld)

    // Path + Name.world
    bool Load(const std::string&);
    // Path + Name.world
    bool Save(const std::string&);

    void OnEntityCreate(CEntity*);
    void OnEntityDestroy(CEntity*);
private:
    int GetNextID();
private:
    int IDPool = 0;
};
