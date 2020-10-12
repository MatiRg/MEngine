#pragma once
#include "EngineModule.hpp"
#include <cstdint>
#include <vector>

class CImGUI;

enum class EPerfCounter
{
    Begin = 0,
    Event,
    Update,
    LateUpdate,
    UI,
    Rendering,
    End // Always last
};

class CPerformanceCounter final: public IEngineModule 
{
public:
    CPerformanceCounter(CImGUI*);
    ~CPerformanceCounter();

    ENGINE_MODULE(CPerformanceCounter)

    void OnUpdate(const float) override;
    void OnRender() override;
    void OnGUI() override;

    void Toggle() { Visible = !Visible; }
    void SetTime(const EPerfCounter Counter, const uint32_t Time) { Counters[static_cast<std::size_t>(Counter)] = Time; }
private:
    CImGUI* UI;
    bool Visible = false;
    std::vector<uint32_t> Counters;
	float TimeTotal = 0.0f;
	float Frames = 0.0f;
};
