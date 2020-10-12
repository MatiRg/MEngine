#pragma once
#include <string>

struct SEngineParams
{
    int Width = 800;
    int Height = 600;
    std::string Caption = "Engine";
    bool VSync = true;
    bool UseAudio = true;
    uint32_t Ticks = 60u;
};
