#pragma once
#include "Core/Types.hpp"

/**
 * \brief Engine Main Function that is called from int main() which is hidden in Engine Library
 * \param Args Command Line Arguments
 * \return Will return 0 on success or any other number on error
 */
extern int EngineMain(const StringVec& Args);

/**
 * \brief Macro that can be used for implementing EngineMain
 * \param Class Class Name that derrivied from CApp
 * \param File Name of the File where Info, Warning, Error Logs etc. will be reported
 *
 */
#define ENTRY_POINT(Class, File) int EngineMain(const StringVec& Cmd) \
{ \
    GetLog().AddSink( std::make_shared<CFileSink>( File ) ); \
    Class GameApp; \
    return GameApp.Run( Cmd ); \
}

