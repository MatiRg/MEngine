Add CMaterial Options -> Texture Filter/Ani, Double Sided, Blend Mode/Enabled, Depth Mode/Enabled, Pass Type -> Solid or Transparent
CMaterial -> Serialize Rest Variables
Rework 2D Rendering to 3D rendering -> Scene Module Part CCamera2D(Ortho) to CCamera
add reload method to IResource
Proper Physics Update
Support some components multiple, some unique add static method bool CanMultiple() - defined by macro COMPONENT; CreateComponent will return if only one or unique per entity
Rework or Remove 2D Support -> See Sprite2D shader

Addy Pure ECS System

Updatables Loaded from optional XML file "Startup.xml" : 
<Updatable Name="aa" [Container="aaa"] /> -- No Container Global
<Start Container="aaa" /> -- No Container Global start


1.Particle Effects
1.Add IComponent -> void OnAdd, void OnRemove to Entity ???

Pre Thesis

1.Project -> Save Session: Opened Scene, Selected Object, Asset Folder
1.Map Ediotr, Improve ImGUI Integration
1.Make Transform editable by mouse -> CGizmos
1.OverlayDisplay Error + Logger + Console ?
1.Create CInputManager -> IAxis -> CAxis, CMouseAxis, IAction -> CKeyboardAction, CMouseAction
1.Enable/Disable Components

- Never Ending Tasks -
1.Rework Entiry Engine Architecture use modern C++ seee Templates, tuples, template static counters, algorithms
1.Optimize Rendering, Containers, Algorithms
1.Update C++ Code, eg. std::optional
1.Use Nomral variable names (not a in funcs or as local)
1.Use modern C++ see std::tuples, std::pair ... etc, templates
1.Add final to classes
1.Check C++ Container usage see. map -> unordered_map
- Never Ending Tasks -

1.Add to Utils NS NormalizePath(Std::string&) - replace all \ with /
1.Reset -> OnReset; while(Run) { if(Reset) Updater->Reset()(All Upodatables/Modules) ... }
1.Resources - Reload method nad it`s usage maybe weak_ptr) - Like SDLSurface, Maybe Scripts
1.USE AngelScript as Script to Relaoding
1.Add Debug Window to Engine RUntime(C++) for realaoding
1.Batches are amalgmated in CRenderer2D.Render() into MultiSet(CDrawBatch, IDrawCommand) -> Set State Here ,Make campatibile with 3D

1.Tiled Map Support
1.Use Full Entity Component System
3.Make Textures PoW2
5.Make engine thread safe
7.Rework Event System -> LowLevel & HighLevel