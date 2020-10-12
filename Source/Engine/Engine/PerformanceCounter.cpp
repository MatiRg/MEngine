#include "PerformanceCounter.hpp"
#include "../Core/Utils.hpp"
#include "../Core/Log.hpp"
#include "../UI/ImGUI.hpp"

namespace
{
    std::string Helper(const EPerfCounter x)
    {
        switch(x)
        {
        case EPerfCounter::Begin:
            return "Begin Frame";
        case EPerfCounter::Event:
            return "Event Processing";
        case EPerfCounter::Update:
            return "Update";
        case EPerfCounter::LateUpdate:
            return "Late Update";
        case EPerfCounter::UI:
            return "UI";
        case EPerfCounter::Rendering:
            return "Rendering";
        case EPerfCounter::End:
            return "End Frame";
        default:
            return "";
        }
    }
}

CPerformanceCounter::CPerformanceCounter(CImGUI* ImGUI):
    IEngineModule( "Performance Counter" ),
    UI(ImGUI)
{
    Counters.resize( static_cast<std::size_t>(EPerfCounter::End) + 1, 0u );
}

CPerformanceCounter::~CPerformanceCounter()
{
}

void CPerformanceCounter::OnUpdate(const float DT)
{
    TimeTotal += DT;
}

void CPerformanceCounter::OnGUI()
{
    if( !Visible ) 
    {
        return;
    }

    UI->Begin("Performance Counter");
    for(std::size_t i = 0; i < Counters.size(); ++i)
    {
        UI->Text( Helper(static_cast<EPerfCounter>(i)) + ": " + std::to_string(Counters[i]) + " ms" );
    }
    UI->Separator();
    UI->Text( "FPS: "+std::to_string(Frames/TimeTotal) );
    UI->End();
}

void CPerformanceCounter::OnRender()
{
    ++Frames;
}