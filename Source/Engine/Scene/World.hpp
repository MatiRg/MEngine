#pragma once
#include "Entity.hpp"
#include "../Math/Color.hpp"

class CEngine;

/**
  \class CWorld
  \brief Root where all objects are created
*/
class CWorld final: public CEntity
{
public:
    CWorld(CEngine*);
    ~CWorld();

    ENTITY(CWorld)

    //! Sets world ambient light color
    void SetAmbientColor(const Color& v) { AmbientColor = v; }
    //! Gets world ambient light color
    const Color& GetAmbientColor() const { return AmbientColor; }

    void OnRender() override;

    //! Load world from given file
    bool Load(const std::string&);
    //! Save world to given file
    bool Save(const std::string&);

    void OnEntityCreate(CEntity*);
    void OnEntityDestroy(CEntity*);
private:
    int GetNextID();
private:
    int IDPool = 0;
    Color AmbientColor = Color(0.1f, 0.1f, 0.1f);
};
