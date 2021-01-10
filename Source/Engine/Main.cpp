#include "Main.hpp"
#include "Core/Log.hpp"
#include "System/MemoryManager.hpp"

int main(int Argc, char* Argv[])
{
    #ifdef DEV_BUILD
    GetLog().AddSink( std::make_shared<CStdSink>() );
    #endif // DEV_BUILD

    int ExitCode = 1;
    try
    {
        StringVec Cmd;
        for(int i = 1; i < Argc; ++i) // ignore program name
        {
            Cmd.push_back( Argv[i] );
        }

        ExitCode = EngineMain( Cmd );
    }
    catch(const std::exception& Exception)
    {
        LOG(ESeverity::Fatal) << Exception.what() << "\n";
    }
    catch(...)
    {
        LOG(ESeverity::Fatal) << "Unknown Exception\n";
    }
    
    GetMemoryManager().Report();

    return ExitCode;
}