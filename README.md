# Engine 
Simple and Crude 2D/3D Game Engine made during Engineering Degree.Inspired by HPL2, Unity5 and Urho3D.
## Installing
See [INSTALL.md](INSTALL.md)
## Features
Features Tree Scene Hierarchy and EC(Entity Component) like System, 2D/3D Sound Playback, Resource Manager, 
File System support, XML Support, Box2d Integration, Bullet3 Integration, OpenGL 4.6 Renderer and ImGUI Ssupport.
### Core
 * Supports XML Files
 * Supports Scene System like to Unity 5
 * Scene System supports custom Entities and Components
 * Extendable Resource System
 * ImGUI Support
### Dev Build
Turned on/off in CMake Options.
 * F10 - Performance Counters - How munch each Engine Stages takes time
 * F11 - ImGUI Demo
### Graphics
 * Basic Debug Drawing
 * Basic 2D Drawing - Rework/Remove
 * Basic 3D Rendering
 * Supports Basic User Created Post Effects
 * Supports Directional Lights and Ambient Lights
 * Supports Model Loading using Assimp
### Audio Module
Audio Files Can be converted with Audacity for example. 
 * Supports Loading Sounds to Memory or Streaming from Disk
 * Supports 2D/3D sound playback
 * 3D Sound play back is supported only for Mono Sounds
 * Supported Formats: WAV and OGG 
### Samples
Engine comes with several samples for easier use.
 * Hello World(C++) - Empty Application
 * Updatables(C++) - How to manage and create Updatables(Game States)
 * Engine Modules(C++) - How to manage and create User Engine Modules
 * Scene Creation(C++) - How to Create World and work with
 * Audio Playback(C++) - How to work with sound
 * Post Effect(C++) - How to work with Post Effect stack
 * Sky(C++) - How to work create Sky Dome
 * Physics(C++) - How to work with Physics Engine
 * Labirynth(C++) - Simple 3D Game
 * Test2D(C++) - Simple 2D Example: Features Simple Game and Physics 2D Test - Rework/Remove
## Documentation
Documention consists of two parts:
 * Doxygen generated part for Classes, functions etc. 
 * Typical documentation and samples with includes this file.
 * See [DOCUMENTATION.md](DOCUMENTATION.md)
 * See [Doxygen.html](Doxygen.html)
## Components
Source Directories
 * Engine(C++) - Engine Code - Base|Shared
 * Samples(C++) - Sample Application
 * Tools(C++) - Tools to use with Engine
## Contribution
If You want to help with development please see these points below.
### Style Guide
See [STYLEGUIDE.md](STYLEGUIDE.md)
## Third Party and Credits
See [THIRDPARTY.md](THIRDPARTY.md)
## License
See [LICENSE.txt](LICENSE.txt)
## Authors
Mateusz Rugor