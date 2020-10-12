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

add_subdirectory(ThirdParty/Common/box2d)

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
	Source/Engine/Graphics/Drawer2D.cpp
	Source/Engine/Graphics/Drawer2D.hpp
	Source/Engine/Graphics/Font.cpp
	Source/Engine/Graphics/Font.hpp
	Source/Engine/Graphics/Graphics.hpp
	Source/Engine/Graphics/Renderable2D.cpp
	Source/Engine/Graphics/Renderable2D.hpp
	Source/Engine/Graphics/Renderer2D.cpp
	Source/Engine/Graphics/Renderer2D.hpp
	Source/Engine/Graphics/Shader.cpp
	Source/Engine/Graphics/Shader.hpp
	Source/Engine/Graphics/Surface.hpp
	Source/Engine/Graphics/Texture2D.cpp
	Source/Engine/Graphics/Texture2D.hpp
	Source/Engine/Graphics/Vertex2D.hpp
	Source/Engine/Graphics/VertexBuffer.hpp
	Source/Engine/Graphics/Window.hpp
	Source/Engine/Implementation/NullAudio/NullAudio.cpp
	Source/Engine/Implementation/NullAudio/NullAudio.hpp
	Source/Engine/Implementation/NullAudio/NullSound.cpp
	Source/Engine/Implementation/NullAudio/NullSound.hpp
	Source/Engine/Implementation/NullAudio/NullSoundData.cpp
	Source/Engine/Implementation/NullAudio/NullSoundData.hpp
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
	Source/Engine/Implementation/OALAudio.cpp
	Source/Engine/Implementation/OALAudio.hpp
	Source/Engine/Implementation/OALSound.cpp
	Source/Engine/Implementation/OALSound.hpp
	Source/Engine/Implementation/OALSoundData.cpp
	Source/Engine/Implementation/OALSoundData.hpp
	Source/Engine/Implementation/OALUtils.cpp
	Source/Engine/Implementation/OALUtils.hpp
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
	Source/Engine/Math/AABB.hpp
	Source/Engine/Math/Color.hpp
	Source/Engine/Math/Functions.cpp
	Source/Engine/Math/Functions.hpp
	Source/Engine/Math/Matrix3.hpp
	Source/Engine/Math/Matrix4.hpp
	Source/Engine/Math/Rect2.hpp
	Source/Engine/Math/Vector2.hpp
	Source/Engine/Math/Vector3.hpp
	Source/Engine/Math/Vector4.hpp
	Source/Engine/Physics/Box2DUtils.hpp
	Source/Engine/Physics/BoxCollider2D.cpp
	Source/Engine/Physics/BoxCollider2D.hpp
	Source/Engine/Physics/Contact2D.hpp
	Source/Engine/Physics/Physics2D.cpp
	Source/Engine/Physics/Physics2D.hpp
	Source/Engine/Physics/PhysicsWorld2D.cpp
	Source/Engine/Physics/PhysicsWorld2D.hpp
	Source/Engine/Physics/RigidBody2D.cpp
	Source/Engine/Physics/RigidBody2D.hpp
	Source/Engine/Resources/Resource.cpp
	Source/Engine/Resources/Resource.hpp
	Source/Engine/Resources/ResourceFactory.hpp
	Source/Engine/Resources/Resources.cpp
	Source/Engine/Resources/Resources.hpp
	Source/Engine/Scene/Components/AudioListener2D.hpp
	Source/Engine/Scene/Components/AudioListener2D.cpp
	Source/Engine/Scene/Components/AudioSource2D.hpp
	Source/Engine/Scene/Components/AudioSource2D.cpp
	Source/Engine/Scene/Components/Camera2D.hpp
	Source/Engine/Scene/Components/Camera2D.cpp
	Source/Engine/Scene/Components/RectRenderer2D.hpp
	Source/Engine/Scene/Components/RectRenderer2D.cpp
	Source/Engine/Scene/Components/StaticSprite2D.hpp
	Source/Engine/Scene/Components/StaticSprite2D.cpp
	Source/Engine/Scene/Component.cpp
	Source/Engine/Scene/Component.hpp
	Source/Engine/Scene/Entity2D.cpp
	Source/Engine/Scene/Entity2D.hpp
	Source/Engine/Scene/Event.hpp
	Source/Engine/Scene/EventManager.hpp
	Source/Engine/Scene/Scene.cpp
	Source/Engine/Scene/Scene.hpp
	Source/Engine/Scene/Transform2D.cpp
	Source/Engine/Scene/Transform2D.hpp
	Source/Engine/Scene/World2D.cpp
	Source/Engine/Scene/World2D.hpp
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

set(SOURCES_HELLO
	Source/HelloWorld/EntryPoint.cpp
	Source/HelloWorld/Globals.hpp
	Source/HelloWorld/MainApp.cpp
	Source/HelloWorld/MainApp.hpp
	Source/HelloWorld/MainMenu.cpp
	Source/HelloWorld/MainMenu.hpp
	Source/HelloWorld/PhysicsPlay.cpp
	Source/HelloWorld/PhysicsPlay.hpp
	Source/HelloWorld/PlayGround.cpp
	Source/HelloWorld/PlayGround.hpp
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
add_executable(HelloWorld ${SOURCES_HELLO})

if(DEV_BUILD)
	set(PPDEFS -DDEV_BUILD)
	target_compile_definitions(MyEngine PUBLIC ${PPDEFS})
	target_compile_definitions(HelloWorld PUBLIC ${PPDEFS})
endif()

# Header only libraries
set(INC_COMMON ThirdParty/Common ThirdParty/Common/box2d/include) 
target_include_directories(MyEngine PUBLIC ${INC_COMMON})
target_include_directories(HelloWorld PUBLIC ${INC_COMMON})

if("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
    message(STATUS "Windows system")
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		set(WIN_PATH_LIBS ThirdParty/Win64-MSVC)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
		set(WIN_PATH_LIBS ThirdParty/Win64-MinGW)
	endif()
	
	set(INC_BASE ${WIN_PATH_LIBS}/include)

	target_include_directories(MyEngine PUBLIC ${INC_BASE})
	target_include_directories(HelloWorld PUBLIC ${INC_BASE})

	link_directories(${WIN_PATH_LIBS}/lib)
	
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		set(LIBS SDL2main SDL2 SDL2_image SDL2_ttf OpenAL32)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
		set(LIBS SDL2main SDL2.dll SDL2_image.dll SDL2_ttf.dll openal.dll)
	endif()
	
	file(GLOB WIN_DLL_LIB "${WIN_PATH_LIBS}/bin/*.dll")
    add_custom_target(copy-dll-files ALL
        COMMAND cmake -E copy ${WIN_DLL_LIB} ${CMAKE_BINARY_DIR}
        DEPENDS ${MY_TARGET})
elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    message(STATUS "Linux system")
    set(LIBS SDL2 SDL2_image SDL2_ttf openal dl)
else()
	message(FATAL_ERROR "Unknown System.")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(LIBS 
		${LIBS}
		stdc++fs
	)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(LIBS 
		${LIBS}
		c++fs
	)
endif()

set(LIBS 
	MyEngine 
	box2d
	${LIBS}
)

target_link_libraries(HelloWorld ${LIBS})

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
	set(CMP_GNU -Wall -Wextra -Wunreachable-code -Wpedantic)

    target_compile_options(MyEngine PRIVATE ${CMP_GNU})
    target_compile_options(HelloWorld PRIVATE ${CMP_GNU})
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMP_CLN -Wweak-vtables -Wexit-time-destructors -Wglobal-constructors -Wmissing-noreturn)

    target_compile_options(MyEngine PRIVATE ${CMP_CLN})
    target_compile_options(HelloWorld PRIVATE ${CMP_CLN})
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(CMP_MSV /W4 /w44265 /w44062 /wd4201 /wd4458 /wd4996 /wd4244 /wd4061 /wd4456 /wd4457 /wd4701 /wd4267 /wd4245 /wd4100)

    target_compile_options(MyEngine PRIVATE ${CMP_MSV})
    target_compile_options(HelloWorld PRIVATE ${CMP_MSV})
endif()
