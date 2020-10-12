# Installation
Welcome to Engine Installation guide.

## Options
 * DEV_BUILD - Enables debug options - Performance Counter - F10, Enables imgui demo window - F11
 
## Windows
Only x64 architecture is supported.
### Requirements
 * CMake
 * Visual Studio Community 2019
 * Source Code
### Compilation
 * Open CMake GUI
 * Select folder with 'CMakeLists.txt' file.
 * Select output directory, recomended thing is to create 'Build' folder.
 * Click `Configure`
 * Select Generator - `Visual Studio 16 2019`
 * Select Optional Platform Generator - `x64`
 * Click `Finish`
 * When configuring is done, click `Generate`, now you can close CMake Gui
 * Open solution in folder specified before - `Engine.sln`
 * Now you can build solution
 * There two main projects in solution: `MyEngine` is engine static library and `HelloWorld` is sample program which you can run

## Linux
 * TO DO