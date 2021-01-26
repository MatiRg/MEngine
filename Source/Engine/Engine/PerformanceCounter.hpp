#pragma once
#include "EngineModule.hpp"
#include "../Core/Types.hpp"

class CImGUI;
class ISystem;
class CPerformanceCounters;

enum class EPerformanceCounter
{
    Begin = 0,
    Event,
    Update,
    LateUpdate,
    ImGUI,
    OnRender,
    Render2D,
    Render3D,
    RenderImGUI,
    End // Always last
};

/**
  \class CPerformanceCounter
  \brief Class used for time measure.Create it int enclosed scope, result will saved in CPerformanceCounters class when this class is destroyed.
*/
class CPerformanceCounter final : public NonCopyableMovable
{
public:
    /**
     * \brief Construct CPerformanceCounter with given name(from enum for Engine Stages)
     * \param Type Enum Type
     * \param aCounter CPerformanceCounters Poiner
     */
    CPerformanceCounter(const EPerformanceCounter Type, CPerformanceCounters* aCounter);
    /**
     * \brief Construct CPerformanceCounter with given name
     * \param aName Name
     * \param aCounter CPerformanceCounters Poiner
     */
    CPerformanceCounter(const std::string& aName, CPerformanceCounters* aCounter);
    //! When destoroyed result is saved in CPerformanceCounters under given name
    ~CPerformanceCounter();
private:
    CPerformanceCounters* Counter = nullptr;
    std::string Name;
    uint32_t StartTime = 0u;
};

class CPerformanceCounters final: public IEngineModule 
{
public:
    CPerformanceCounters(CImGUI*, ISystem*);
    ~CPerformanceCounters();

    ENGINE_MODULE(CPerformanceCounters)

    ISystem* GetSystem() const { return System; }

    void OnGUI() override;

    void StartMeasurment();
    void EndMeasurment();

    void Toggle();

    void SetTime(const std::string& Name, const uint32_t Time);
private:
    struct SCounter
    {
        SCounter(const std::string& aName, const uint32_t aTime):
            Name(aName),
            Time(aTime)
        {
        }

        std::string Name;
        uint32_t Time;
    };
private:
    CImGUI* UI = nullptr;
    ISystem* System = nullptr;
    bool Visible = false;
    std::vector<SCounter> Counters;
    std::vector<SCounter> ToDraw;
    //
    uint32_t StartLoop = 0u;
    std::size_t FPSLast = 0u;
    std::vector<float> FPSRecords;
};
