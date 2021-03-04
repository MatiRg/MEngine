cmake_minimum_required(VERSION 3.5)

project(Engine CXX)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "In-source builds are not permitted.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	message(STATUS "Detected Clang compiler")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	message(STATUS "Detected GCC compiler")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	message(STATUS "Detected MSVC compiler")
else()
	message(FATAL_ERROR "Unsupported compiler.")
endif()

option(DEV_BUILD "Enable developer build." ON)

if("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_custom_target(copy-runtime-files ALL
    COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/Data ${CMAKE_BINARY_DIR}/Data
    DEPENDS ${MY_TARGET})
	
add_custom_target(copy-core-files ALL
    COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/Core ${CMAKE_BINARY_DIR}/Core
    DEPENDS ${MY_TARGET})

add_custom_target(copy-engine-config ALL
    COMMAND cmake -E copy ${CMAKE_SOURCE_DIR}/Engine.xml ${CMAKE_BINARY_DIR}/Engine.xml
    DEPENDS ${MY_TARGET})
	
set(SOURCES_BASE
	Source/Engine/Audio/Audio.hpp
	Source/Engine/Audio/AudioHandler.cpp
	Source/Engine/Audio/AudioHandler.hpp
	Source/Engine/Audio/Sound.hpp
	Source/Engine/Audio/SoundData.cpp
	Source/Engine/Audio/SoundData.hpp
	Source/Engine/Core/BinaryFile.hpp
	Source/Engine/Core/Log.cpp
	Source/Engine/Core/Log.hpp
	Source/Engine/Core/NonCopyable.hpp
	Source/Engine/Core/Platform.hpp
	Source/Engine/Core/Types.hpp
	Source/Engine/Core/Utils.cpp
	Source/Engine/Core/Utils.hpp
	Source/Engine/Core/XML.cpp
	Source/Engine/Core/XML.hpp
	Source/Engine/Engine/App.cpp
	Source/Engine/Engine/App.hpp
	Source/Engine/Engine/EngineParams.hpp
	Source/Engine/Engine/Engine.cpp
	Source/Engine/Engine/Engine.hpp
	Source/Engine/Engine/EngineUpdater.cpp
	Source/Engine/Engine/EngineUpdater.hpp
	Source/Engine/Engine/Config.cpp
	Source/Engine/Engine/Config.hpp
	Source/Engine/Engine/Context.hpp
	Source/Engine/Engine/EngineModule.hpp
	Source/Engine/Engine/PerformanceCounter.cpp
	Source/Engine/Engine/PerformanceCounter.hpp
	Source/Engine/Engine/Updatable.hpp
	Source/Engine/Engine/UserUpdater.cpp
	Source/Engine/Engine/UserUpdater.hpp
	Source/Engine/Graphics/DebugDrawer.cpp
	Source/Engine/Graphics/DebugDrawer.hpp
	Source/Engine/Graphics/Drawer2D.cpp
	Source/Engine/Graphics/Drawer2D.hpp
	Source/Engine/Graphics/Font.cpp
	Source/Engine/Graphics/Font.hpp
	Source/Engine/Graphics/FrameBuffer.hpp
	Source/Engine/Graphics/GraphicsTypes.hpp
	Source/Engine/Graphics/Graphics.hpp
	Source/Engine/Graphics/Light.cpp
	Source/Engine/Graphics/Light.hpp
	Source/Engine/Graphics/Material.cpp
	Source/Engine/Graphics/Material.hpp
	Source/Engine/Graphics/Model.cpp
	Source/Engine/Graphics/Model.hpp
	Source/Engine/Graphics/PostEffect.cpp
	Source/Engine/Graphics/PostEffect.hpp
	Source/Engine/Graphics/Renderable2D.cpp
	Source/Engine/Graphics/Renderable2D.hpp
	Source/Engine/Graphics/Renderable3D.cpp
	Source/Engine/Graphics/Renderable3D.hpp
	Source/Engine/Graphics/Renderer2D.cpp
	Source/Engine/Graphics/Renderer2D.hpp
	Source/Engine/Graphics/Renderer3D.cpp
	Source/Engine/Graphics/Renderer3D.hpp
	Source/Engine/Graphics/Shader.cpp
	Source/Engine/Graphics/Shader.hpp
	Source/Engine/Graphics/Surface.hpp
	Source/Engine/Graphics/Texture2D.cpp
	Source/Engine/Graphics/Texture2D.hpp
	Source/Engine/Graphics/Vertex2D.hpp
	Source/Engine/Graphics/VertexBuffer.cpp
	Source/Engine/Graphics/VertexBuffer.hpp
	Source/Engine/Graphics/Window.hpp
	Source/Engine/Implementation/NullAudio/NullAudio.cpp
	Source/Engine/Implementation/NullAudio/NullAudio.hpp
	Source/Engine/Implementation/NullAudio/NullSound.cpp
	Source/Engine/Implementation/NullAudio/NullSound.hpp
	Source/Engine/Implementation/NullAudio/NullSoundData.cpp
	Source/Engine/Implementation/NullAudio/NullSoundData.hpp
	Source/Engine/Implementation/SFMLAudio/SFMLAudio.cpp
	Source/Engine/Implementation/SFMLAudio/SFMLAudio.hpp
	Source/Engine/Implementation/SFMLAudio/SFMLSound.cpp
	Source/Engine/Implementation/SFMLAudio/SFMLSound.hpp
	Source/Engine/Implementation/SFMLAudio/SFMLSoundData.cpp
	Source/Engine/Implementation/SFMLAudio/SFMLSoundData.hpp
	Source/Engine/Implementation/OGL/OGLFrameBuffer.cpp
	Source/Engine/Implementation/OGL/OGLFrameBuffer.hpp
	Source/Engine/Implementation/OGL/OGLGraphics.cpp
	Source/Engine/Implementation/OGL/OGLGraphics.hpp
	Source/Engine/Implementation/OGL/OGLShader.cpp
	Source/Engine/Implementation/OGL/OGLShader.hpp
	Source/Engine/Implementation/OGL/OGLTexture2D.cpp
	Source/Engine/Implementation/OGL/OGLTexture2D.hpp
	Source/Engine/Implementation/OGL/OGLUtils.cpp
	Source/Engine/Implementation/OGL/OGLUtils.hpp
	Source/Engine/Implementation/OGL/OGLVertexBuffer.cpp
	Source/Engine/Implementation/OGL/OGLVertexBuffer.hpp
	Source/Engine/Implementation/Box2D/Box2DUtils.hpp
	Source/Engine/Implementation/Box2D/Box2DPhysics2D.cpp
	Source/Engine/Implementation/Box2D/Box2DPhysics2D.hpp
	Source/Engine/Implementation/Box2D/Box2DCollisionShape2D.cpp
	Source/Engine/Implementation/Box2D/Box2DCollisionShape2D.hpp
	Source/Engine/Implementation/Box2D/Box2DPhysicsWorld2D.cpp
	Source/Engine/Implementation/Box2D/Box2DPhysicsWorld2D.hpp
	Source/Engine/Implementation/Box2D/Box2DRigidBody2D.cpp
	Source/Engine/Implementation/Box2D/Box2DRigidBody2D.hpp	
	Source/Engine/Implementation/Bullet/BulletPhysics3D.cpp
	Source/Engine/Implementation/Bullet/BulletPhysics3D.hpp
	Source/Engine/Implementation/Bullet/BulletCollisionShape3D.cpp
	Source/Engine/Implementation/Bullet/BulletCollisionShape3D.hpp
	Source/Engine/Implementation/Bullet/BulletPhysicsWorld3D.cpp
	Source/Engine/Implementation/Bullet/BulletPhysicsWorld3D.hpp
	Source/Engine/Implementation/Bullet/BulletRigidBody3D.cpp
	Source/Engine/Implementation/Bullet/BulletRigidBody3D.hpp
	Source/Engine/Implementation/Bullet/BulletUtils.hpp
	Source/Engine/Implementation/SDLEventQueue.cpp
	Source/Engine/Implementation/SDLEventQueue.hpp
	Source/Engine/Implementation/SDLFont.cpp
	Source/Engine/Implementation/SDLFont.hpp
	Source/Engine/Implementation/SDLInput.cpp
	Source/Engine/Implementation/SDLInput.hpp
	Source/Engine/Implementation/SDLContext.cpp
	Source/Engine/Implementation/SDLContext.hpp
	Source/Engine/Implementation/SDLSurface.cpp
	Source/Engine/Implementation/SDLSurface.hpp
	Source/Engine/Implementation/SDLSystem.cpp
	Source/Engine/Implementation/SDLSystem.hpp
	Source/Engine/Implementation/SDLWindow.cpp
	Source/Engine/Implementation/SDLWindow.hpp
	Source/Engine/Math/Color.cpp
	Source/Engine/Math/Color.hpp
	Source/Engine/Math/Functions.cpp
	Source/Engine/Math/Functions.hpp
	Source/Engine/Math/IntVector2.cpp
	Source/Engine/Math/IntVector2.hpp
	Source/Engine/Math/Matrix3.cpp
	Source/Engine/Math/Matrix3.hpp
	Source/Engine/Math/Matrix4.cpp
	Source/Engine/Math/Matrix4.hpp
	Source/Engine/Math/Quaternion.cpp
	Source/Engine/Math/Quaternion.hpp
	Source/Engine/Math/Rect2.cpp
	Source/Engine/Math/Rect2.hpp
	Source/Engine/Math/Rect3.cpp
	Source/Engine/Math/Rect3.hpp
	Source/Engine/Math/Transform.cpp
	Source/Engine/Math/Transform.hpp
	Source/Engine/Math/Vector2.cpp
	Source/Engine/Math/Vector2.hpp
	Source/Engine/Math/Vector3.cpp
	Source/Engine/Math/Vector3.hpp
	Source/Engine/Math/Vector4.cpp
	Source/Engine/Math/Vector4.hpp
	Source/Engine/Physics/CollisionShape2D.cpp
	Source/Engine/Physics/CollisionShape2D.hpp
	Source/Engine/Physics/Contact2D.hpp
	Source/Engine/Physics/Physics2D.cpp
	Source/Engine/Physics/Physics2D.hpp
	Source/Engine/Physics/PhysicsWorld2D.cpp
	Source/Engine/Physics/PhysicsWorld2D.hpp
	Source/Engine/Physics/RigidBody2D.cpp
	Source/Engine/Physics/RigidBody2D.hpp
	Source/Engine/Physics/Physics3D.cpp
	Source/Engine/Physics/Physics3D.hpp
	Source/Engine/Physics/CollisionShape3D.cpp
	Source/Engine/Physics/CollisionShape3D.hpp
	Source/Engine/Physics/PhysicsWorld3D.cpp
	Source/Engine/Physics/PhysicsWorld3D.hpp
	Source/Engine/Physics/RigidBody3D.cpp
	Source/Engine/Physics/RigidBody3D.hpp
	Source/Engine/Physics/Contact3D.hpp
	Source/Engine/Resources/Resource.cpp
	Source/Engine/Resources/Resource.hpp
	Source/Engine/Resources/ResourceManager.cpp
	Source/Engine/Resources/ResourceManager.hpp
	Source/Engine/Resources/Resources.cpp
	Source/Engine/Resources/Resources.hpp
	Source/Engine/Scene/Components/AudioListener.hpp
	Source/Engine/Scene/Components/AudioListener.cpp
	Source/Engine/Scene/Components/AudioSource.hpp
	Source/Engine/Scene/Components/AudioSource.cpp
	Source/Engine/Scene/Components/Camera.hpp
	Source/Engine/Scene/Components/Camera.cpp
	Source/Engine/Scene/Components/Camera2D.hpp
	Source/Engine/Scene/Components/Camera2D.cpp
	Source/Engine/Scene/Components/LightComponent.hpp
	Source/Engine/Scene/Components/LightComponent.cpp
	Source/Engine/Scene/Components/MeshRenderer.hpp
	Source/Engine/Scene/Components/MeshRenderer.cpp
	Source/Engine/Scene/Components/RectRenderer2D.hpp
	Source/Engine/Scene/Components/RectRenderer2D.cpp
	Source/Engine/Scene/Components/StaticSprite2D.hpp
	Source/Engine/Scene/Components/StaticSprite2D.cpp
	Source/Engine/Scene/Components/BoxCollider2D.hpp
	Source/Engine/Scene/Components/BoxCollider2D.cpp
	Source/Engine/Scene/Components/PhysicsWorld2D.hpp
	Source/Engine/Scene/Components/PhysicsWorld2D.cpp
	Source/Engine/Scene/Components/RigidBody2D.hpp
	Source/Engine/Scene/Components/RigidBody2D.cpp
	Source/Engine/Scene/Components/PhysicsWorld3D.hpp
	Source/Engine/Scene/Components/PhysicsWorld3D.cpp
	Source/Engine/Scene/Components/BoxCollider3D.hpp
	Source/Engine/Scene/Components/BoxCollider3D.cpp
	Source/Engine/Scene/Components/RigidBody3D.hpp
	Source/Engine/Scene/Components/RigidBody3D.cpp
	Source/Engine/Scene/Component.cpp
	Source/Engine/Scene/Component.hpp
	Source/Engine/Scene/Entity.cpp
	Source/Engine/Scene/Entity.hpp
	Source/Engine/Scene/Event.hpp
	Source/Engine/Scene/EventManager.hpp
	Source/Engine/Scene/Scene.cpp
	Source/Engine/Scene/Scene.hpp
	Source/Engine/Scene/World.cpp
	Source/Engine/Scene/World.hpp
	Source/Engine/Script/ScriptModule.cpp
	Source/Engine/Script/ScriptModule.hpp
	Source/Engine/Input/Event.hpp
	Source/Engine/Input/EventQueue.hpp
	Source/Engine/Input/EventTypes.hpp
	Source/Engine/Input/Input.hpp
	Source/Engine/System/FileRecord.hpp
	Source/Engine/System/MemoryManager.cpp
	Source/Engine/System/MemoryManager.hpp
	Source/Engine/System/System.hpp
	Source/Engine/UI/ImGUI.cpp
	Source/Engine/UI/ImGUI.hpp
	Source/Engine/UI/ImGUIFlags.hpp
	Source/Engine/UI/ImGUIRenderer.hpp
	Source/Engine/Main.cpp
	Source/Engine/Main.hpp
	Source/Engine/Full.hpp
	ThirdParty/Common/tinyfiledialogs.c
	ThirdParty/Common/tinyfiledialogs.h
	ThirdParty/Common/tinyxml2.cpp
	ThirdParty/Common/tinyxml2.h
	ThirdParty/Common/glad/glad.c
	ThirdParty/Common/glad/glad.h
	ThirdParty/Common/glad/khrplatform.h
	ThirdParty/Common/imgui/imconfig.h
	ThirdParty/Common/imgui/imgui.cpp
	ThirdParty/Common/imgui/imgui.h
	ThirdParty/Common/imgui/imgui_demo.cpp
	ThirdParty/Common/imgui/imgui_draw.cpp
	ThirdParty/Common/imgui/imgui_impl_opengl3.cpp
	ThirdParty/Common/imgui/imgui_impl_opengl3.h
	ThirdParty/Common/imgui/imgui_internal.h
	ThirdParty/Common/imgui/imgui_stdlib.cpp
	ThirdParty/Common/imgui/imgui_stdlib.h
	ThirdParty/Common/imgui/imgui_widgets.cpp
	ThirdParty/Common/imgui/imstb_rectpack.h
	ThirdParty/Common/imgui/imstb_textedit.h
	ThirdParty/Common/imgui/imstb_truetype.h
)

# Here You can list add new source/header files
set(SOURCES_98_LABIRYNTH
	Source/Samples/98_Labirynth/EntryPoint.cpp
	Source/Samples/98_Labirynth/Globals.hpp
	Source/Samples/98_Labirynth/MainApp.cpp
	Source/Samples/98_Labirynth/MainApp.hpp
	Source/Samples/98_Labirynth/Level2.cpp
	Source/Samples/98_Labirynth/Level2.hpp
)

set(SOURCES_99_TEST2D
	Source/Samples/99_Test_2D/EntryPoint.cpp
	Source/Samples/99_Test_2D/Globals.hpp
	Source/Samples/99_Test_2D/MainApp.cpp
	Source/Samples/99_Test_2D/MainApp.hpp
	Source/Samples/99_Test_2D/MainMenu.cpp
	Source/Samples/99_Test_2D/MainMenu.hpp
	Source/Samples/99_Test_2D/PhysicsPlay.cpp
	Source/Samples/99_Test_2D/PhysicsPlay.hpp
	Source/Samples/99_Test_2D/PlayGround.cpp
	Source/Samples/99_Test_2D/PlayGround.hpp
)

set_source_files_properties(ThirdParty/Common/tinyfiledialogs.c PROPERTIES LANGUAGE CXX)
set_source_files_properties(ThirdParty/Common/glad/glad.c PROPERTIES LANGUAGE CXX)

if("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		set(WIN_PATH_LIBS ThirdParty/Win64-MSVC)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
		set(WIN_PATH_LIBS ThirdParty/Win64-MinGW)
	endif()
	link_directories(${WIN_PATH_LIBS}/lib)
endif()

add_library(MyEngine STATIC ${SOURCES_BASE})
#
add_executable(HelloWorld Source/Samples/01_HelloWorld/Main.cpp)
add_executable(UpdatablesSample Source/Samples/02_Updatables/Main.cpp)
add_executable(EngineModulesSample Source/Samples/03_EngineModules/Main.cpp)
add_executable(SceneCreationSample Source/Samples/04_SceneCreation/Main.cpp)
add_executable(AudioPlaybackSample Source/Samples/05_AudioPlayback/Main.cpp)
add_executable(PostEffectSample Source/Samples/06_PostEffect/Main.cpp)
add_executable(SkySample Source/Samples/07_Sky/Main.cpp)
add_executable(PhysicsSample Source/Samples/08_Physics/Main.cpp)
#
add_executable(MathSandbox Source/Samples/97_MathSandbox/Main.cpp)
add_executable(Test2D ${SOURCES_99_TEST2D})
add_executable(Labirynth ${SOURCES_98_LABIRYNTH})

if(DEV_BUILD)
	set(PPDEFS -DDEV_BUILD)
	target_compile_definitions(MyEngine PUBLIC ${PPDEFS})
	#
	target_compile_definitions(HelloWorld PUBLIC ${PPDEFS})
	target_compile_definitions(UpdatablesSample PUBLIC ${PPDEFS})
	target_compile_definitions(EngineModulesSample PUBLIC ${PPDEFS})
	target_compile_definitions(SceneCreationSample PUBLIC ${PPDEFS})
	target_compile_definitions(AudioPlaybackSample PUBLIC ${PPDEFS})
	target_compile_definitions(PostEffectSample PUBLIC ${PPDEFS})
	target_compile_definitions(SkySample PUBLIC ${PPDEFS})
	target_compile_definitions(PhysicsSample PUBLIC ${PPDEFS})
	#
	target_compile_definitions(MathSandbox PUBLIC ${PPDEFS})
	target_compile_definitions(Test2D PUBLIC ${PPDEFS})
	target_compile_definitions(Labirynth PUBLIC ${PPDEFS})
endif()

# Header only libraries and Common
set(INC_COMMON ThirdParty/Common Source) 

if("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
    message(STATUS "Windows system")
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		set(WIN_PATH_LIBS ThirdParty/Win64-MSVC)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
		set(WIN_PATH_LIBS ThirdParty/Win64-MinGW)
	endif()
	
	set(INC_BASE ${WIN_PATH_LIBS}/include)
	set(INC_BASE ${INC_COMMON} ${INC_BASE})

	link_directories(${WIN_PATH_LIBS}/lib)
	
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		set(LIBS SDL2main SDL2 SDL2_image SDL2_ttf sfml-audio sfml-system box2d assimp-vc142-mt BulletCollision BulletDynamics BulletInverseDynamics BulletSoftBody LinearMath)
		set(LIBS_DBG SDL2main SDL2 SDL2_image SDL2_ttf sfml-audio-d sfml-system-d box2d assimp-vc142-mt BulletCollision_Debug BulletDynamics_Debug BulletInverseDynamics_Debug BulletSoftBody_Debug LinearMath_Debug)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
		set(LIBS SDL2main SDL2.dll SDL2_image.dll SDL2_ttf.dll openal.dll) # TO DO
		set(LIBS_DBG ${LIBS})
	endif()
	
	file(GLOB WIN_DLL_LIB "${WIN_PATH_LIBS}/bin/*.dll")
    add_custom_target(copy-dll-files ALL
        COMMAND cmake -E copy ${WIN_DLL_LIB} ${CMAKE_BINARY_DIR}
        DEPENDS ${MY_TARGET})
elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux") # TO DO
	message(STATUS "Linux system")

	set(INC_BASE ${INC_COMMON})

	set(LIBS SDL2 SDL2_image SDL2_ttf openal dl)
	set(LIBS_DBG ${LIBS})
else()
	message(FATAL_ERROR "Unknown System.")
endif()

target_include_directories(MyEngine PUBLIC ${INC_BASE})
#
target_include_directories(HelloWorld PUBLIC ${INC_BASE})
target_include_directories(UpdatablesSample PUBLIC ${INC_BASE})
target_include_directories(EngineModulesSample PUBLIC ${INC_BASE})
target_include_directories(SceneCreationSample PUBLIC ${INC_BASE})
target_include_directories(AudioPlaybackSample PUBLIC ${INC_BASE})
target_include_directories(PostEffectSample PUBLIC ${INC_BASE})
target_include_directories(SkySample PUBLIC ${INC_BASE})
target_include_directories(PhysicsSample PUBLIC ${INC_BASE})
#
target_include_directories(MathSandbox PUBLIC ${INC_BASE})
target_include_directories(Test2D PUBLIC ${INC_BASE})
target_include_directories(Labirynth PUBLIC ${INC_BASE})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(LIBS 
		${LIBS}
		stdc++fs
	)
	set(LIBS_DBG 
		${LIBS_DBG}
		stdc++fs
	)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(LIBS 
		${LIBS}
		c++fs
	)
	set(LIBS_DBG 
		${LIBS_DBG}
		c++fs
	)
endif()

set(LIBS_DBG 
	MyEngine
	${LIBS_DBG}
)
set(LIBS 
	MyEngine 
	${LIBS}
)
#set(LIBRARIES debug ${LIBS_DBG} optimized ${LIBS}) # Fix
set(LIBRARIES ${LIBS_DBG})

target_link_libraries(HelloWorld ${LIBRARIES})
target_link_libraries(UpdatablesSample ${LIBRARIES})
target_link_libraries(EngineModulesSample ${LIBRARIES})
target_link_libraries(SceneCreationSample ${LIBRARIES})
target_link_libraries(AudioPlaybackSample ${LIBRARIES})
target_link_libraries(PostEffectSample ${LIBRARIES})
target_link_libraries(SkySample ${LIBRARIES})
target_link_libraries(PhysicsSample ${LIBRARIES})
#
target_link_libraries(MathSandbox ${LIBRARIES})
target_link_libraries(Test2D ${LIBRARIES})
target_link_libraries(Labirynth ${LIBRARIES})

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
	set(CMP_GNU -Wall -Wextra -Wunreachable-code -Wpedantic)
	set(CMP_OPT ${CMP_GNU})
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMP_CLN -Wweak-vtables -Wexit-time-destructors -Wglobal-constructors -Wmissing-noreturn)
	set(CMP_OPT ${CMP_CLN})
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(CMP_MSV /W4 /w44265 /w44062 /wd4201 /wd4458 /wd4996 /wd4244 /wd4061 /wd4456 /wd4457 /wd4701 /wd4267 /wd4245 /wd4100)
	set(CMP_OPT ${CMP_MSV})
endif()

target_compile_options(MyEngine PRIVATE ${CMP_OPT})
#
target_compile_options(HelloWorld PRIVATE ${CMP_OPT})
target_compile_options(UpdatablesSample PRIVATE ${CMP_OPT})
target_compile_options(EngineModulesSample PRIVATE ${CMP_OPT})
target_compile_options(SceneCreationSample PRIVATE ${CMP_OPT})
target_compile_options(AudioPlaybackSample PRIVATE ${CMP_OPT})
target_compile_options(PostEffectSample PRIVATE ${CMP_OPT})
target_compile_options(SkySample PRIVATE ${CMP_OPT})
target_compile_options(PhysicsSample PRIVATE ${CMP_OPT})
#
target_compile_options(MathSandbox PRIVATE ${CMP_OPT})
target_compile_options(Test2D PRIVATE ${CMP_OPT})
target_compile_options(Labirynth PRIVATE ${CMP_OPT})
