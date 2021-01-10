#pragma once
#include "Entity.hpp"
#include "../Math/Color.hpp"

class CEngine;

class CWorld final: public CEntity
{
public:
    CWorld(CEngine*);
    ~CWorld();

    ENTITY(CWorld)

    void SetAmbientColor(const Color& v) { AmbientColor = v; }
    const Color& GetAmbientColor() const { return AmbientColor; }

    void OnRender() override;

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
    Color AmbientColor = Color(0.1f, 0.1f, 0.1f);
};
