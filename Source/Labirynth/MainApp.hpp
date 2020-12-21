#pragma once
#include "../Engine/Full.hpp"
#include "Level2.hpp"

class CMainApp: public CApp
{
public:
    CMainApp() = default;
    ~CMainApp() = default;
protected:
    bool Init() override;
    void OnInit() override;
    void Exit() override;
private:
    std::unique_ptr<CLevel2> Level2;
};
