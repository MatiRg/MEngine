#pragma once

// _WIN64   - 64 bit
// _WIN32   - 32 i 64 bit
// _M_X64   - x64 Visual Studio
// __CYGWIN__   - Windows with Cygwin (POSIX)
//#if defined(__WIN32__) || defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
// __linux__     - Linux kernel
// __x86_64__    - x64 GNUC
//#elif defined(__LINUX__) || defined(linux) || defined(__linux) || defined(__linux__)

#if defined(_WIN64) && defined(_M_X64)
#   define WINDOWS_PLATFORM 1
#elif defined(__linux__) && defined(__x86_64__)
#   define LINUX_PLATFORM 1
#else
#   error "Unsupported platform"
#endif

//On Windows, it is necessary for you to flag any methods that you want to expose through P/Invoke to be flagged with __dllexport or __declspec(dllexport).
//__attribute__((dllexport))    - GCC
// https://gcc.gnu.org/wiki/Visibility

#ifdef WINDOWS_PLATFORM
#   define DLL_EXPORT __declspec(dllexport)
#   define DLL_IMPORT __declspec(dllimport)
#else
#   define DLL_EXPORT
#   define DLL_IMPORT
#endif // WINDOWS_PLATFORM
