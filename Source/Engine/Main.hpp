#pragma once
#include <vector>
#include <string>

// Args
extern int EngineMain(const std::vector<std::string>&);

#define ENTRY_POINT(Class, File) int EngineMain(const std::vector<std::string>& Cmd) \
{ \
    GetLog().AddSink( std::make_shared<CFileSink>( File ) ); \
    Class GameApp; \
    return GameApp.Run( Cmd ); \
}

