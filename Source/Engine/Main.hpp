#pragma once
#include "Core/Types.hpp"

// Args
extern int EngineMain(const StringVec&);

#define ENTRY_POINT(Class, File) int EngineMain(const StringVec& Cmd) \
{ \
    GetLog().AddSink( std::make_shared<CFileSink>( File ) ); \
    Class GameApp; \
    return GameApp.Run( Cmd ); \
}

