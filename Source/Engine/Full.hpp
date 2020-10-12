#pragma once

#include "Main.hpp"

#include "Audio/Audio.hpp"
#include "Audio/AudioHandler.hpp"
#include "Audio/SoundData.hpp"
#include "Audio/Sound.hpp"

#include "Core/BinaryFile.hpp"
#include "Core/Log.hpp"
#include "Core/NonCopyable.hpp"
#include "Core/Platform.hpp"
#include "Core/Utils.hpp"
#include "Core/XML.hpp"

#include "Engine/App.hpp"
#include "Engine/EngineParams.hpp"
#include "Engine/Engine.hpp"
#include "Engine/EngineUpdater.hpp"
#include "Engine/Config.hpp"
#include "Engine/Context.hpp"
#include "Engine/EngineModule.hpp"
#include "Engine/PerformanceCounter.hpp"
#include "Engine/Updatable.hpp"
#include "Engine/UserUpdater.hpp"

#include "Graphics/Drawer2D.hpp"
#include "Graphics/Font.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/Renderable2D.hpp"
#include "Graphics/Renderer2D.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Surface.hpp"
#include "Graphics/Texture2D.hpp"
#include "Graphics/Vertex2D.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/Window.hpp"

#include "Input/Event.hpp"
#include "Input/EventQueue.hpp"
#include "Input/EventTypes.hpp"
#include "Input/Input.hpp"

#include "Math/AABB.hpp"
#include "Math/Color.hpp"
#include "Math/Functions.hpp"
#include "Math/Matrix3.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Rect2.hpp"
#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector4.hpp"

#include "Physics/BoxCollider2D.hpp"
#include "Physics/Contact2D.hpp"
#include "Physics/Physics2D.hpp"
#include "Physics/PhysicsWorld2D.hpp"
#include "Physics/RigidBody2D.hpp"

#include "Resources/Resource.hpp"
#include "Resources/ResourceFactory.hpp"
#include "Resources/Resources.hpp"

#include "Scene/Components/AudioListener2D.hpp"
#include "Scene/Components/AudioSource2D.hpp"
#include "Scene/Components/Camera2D.hpp"
#include "Scene/Components/RectRenderer2D.hpp"
#include "Scene/Components/StaticSprite2D.hpp"
#include "Scene/Component.hpp"
#include "Scene/Entity2D.hpp"
#include "Scene/Event.hpp"
#include "Scene/EventManager.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Transform2D.hpp"
#include "Scene/World2D.hpp"

#include "Script/ScriptModule.hpp"

#include "System/FileRecord.hpp"
#include "System/MemoryManager.hpp"
#include "System/System.hpp"

#include "UI/ImGUI.hpp"
#include "UI/ImGUIFlags.hpp"
#include "UI/ImGUIRenderer.hpp"
