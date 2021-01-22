#include "PerformanceCounter.hpp"
#include "../Core/Utils.hpp"
#include "../Core/Log.hpp"
#include "../UI/ImGUI.hpp"
#include "../System/System.hpp"

namespace
{
    std::string PerformanceCounterToString(const EPerformanceCounter x)
    {
        switch(x)
        {
        case EPerformanceCounter::Begin:
            return "Begin Frame";
        case EPerformanceCounter::Event:
            return "Event Processing";
        case EPerformanceCounter::Update:
            return "Update";
        case EPerformanceCounter::LateUpdate:
            return "Late Update";
        case EPerformanceCounter::ImGUI:
            return "ImGUI";
        case EPerformanceCounter::OnRender:
            return "On Render";
        case EPerformanceCounter::Render2D:
            return "Render 2D";
        case EPerformanceCounter::Render3D:
            return "Render 3D";
        case EPerformanceCounter::RenderImGUI:
            return "Render ImGUI";
        case EPerformanceCounter::End:
            return "End Frame";
        default:
            return "";
        }
    }
}

//////////////////////////////////////////////////////////

CPerformanceCounter::CPerformanceCounter(const EPerformanceCounter Type, CPerformanceCounters* aCounter):
    Counter(aCounter),
    Name( PerformanceCounterToString(Type) ),
    StartTime( Counter->GetSystem()->GetTime() )
{
}

CPerformanceCounter::CPerformanceCounter(const std::string& aName, CPerformanceCounters* aCounter):
    Counter(aCounter),
    Name( aName ),
    StartTime( Counter->GetSystem()->GetTime() )
{
}

CPerformanceCounter::~CPerformanceCounter()
{
    Counter->SetTime( Name, Counter->GetSystem()->GetTime() - StartTime );
}

//////////////////////////////////////////////////////////

static constexpr std::size_t FPS_MAX_VALUES = 10u;

CPerformanceCounters::CPerformanceCounters(CImGUI* ImGUI, ISystem* aSystem):
    IEngineModule( "Performance Counters" ),
    UI(ImGUI),
    System(aSystem),
    FPSRecords(FPS_MAX_VALUES, 0.0f)
{
}

CPerformanceCounters::~CPerformanceCounters()
{
}

void CPerformanceCounters::OnGUI()
{
    if( !Visible ) 
    {
        return;
    }

    float Sum = 0.0f;
    for (const auto& i : FPSRecords)
    {
        Sum += i;
    }
    Sum /= static_cast<float>(FPS_MAX_VALUES);

    UI->Begin("Performance Counter");
    for (const auto& i : ToDraw)
    {
        UI->Text(i.Name + ": " + std::to_string(i.Time) + " ms");
    }
    UI->Separator();
    UI->Text( "FPS: "+std::to_string(Sum) );
    UI->End();
}

void CPerformanceCounters::StartMeasurment()
{
    StartLoop = System->GetTime();
}

void CPerformanceCounters::EndMeasurment()
{
    float Difference = (static_cast<float>(System->GetTime()) - static_cast<float>(StartLoop)) / 1000.0f;
    float FPS = 0.0f;
    if( Difference )
    {
        FPS = 1.0f / Difference;
    }
    FPSRecords[FPSLast++] = FPS;
    if (FPSLast >= FPS_MAX_VALUES)
    {
        FPSLast = 0u;
    }
    //
    ToDraw = Counters;
    Counters.clear();
}

void CPerformanceCounters::Toggle()
{ 
    Visible = !Visible; 
}

void CPerformanceCounters::SetTime(const std::string& Name, const uint32_t Time)
{ 
    Counters.emplace_back(Name, Time);
}