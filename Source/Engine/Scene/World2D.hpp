#pragma once
#include "Entity2D.hpp"

class CEngine;

class CWorld2D final: public CEntity2D
{
public:
    CWorld2D(CEngine*);
    ~CWorld2D();

    ENTITY(CWorld2D)

    // Path + Name.world2d
    bool Load(const std::string&);
    // Path + Name.world2d
    bool Save(const std::string&);

    void OnBeginFrame() override;
    void OnUpdate(const float) override;
    void OnLateUpdate(const float) override;
    void OnGUI() override;
    void OnRender() override;
    void OnEndFrame() override;

    void OnEntityCreate(CEntity2D*);
    void OnEntityDestroy(CEntity2D*);
private:
    int GetNextID();
private:
    int IDPool = 0;
};
