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
#include "Core/Types.hpp"
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
#include "Graphics/FrameBuffer.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/Light.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/PostEffect.hpp"
#include "Graphics/Renderable2D.hpp"
#include "Graphics/Renderable3D.hpp"
#include "Graphics/Renderer2D.hpp"
#include "Graphics/Renderer3D.hpp"
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

#include "Math/AABB2.hpp"
#include "Math/AABB3.hpp"
#include "Math/Color.hpp"
#include "Math/Functions.hpp"
#include "Math/Matrix3.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Rect2.hpp"
#include "Math/Rect3.hpp"
#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector4.hpp"

#include "Physics/CollisionShape2D.hpp"
#include "Physics/Contact2D.hpp"
#include "Physics/Physics2D.hpp"
#include "Physics/PhysicsWorld2D.hpp"
#include "Physics/RigidBody2D.hpp"
#include "Physics/Physics3D.hpp"
#include "Physics/CollisionShape3D.hpp"
#include "Physics/PhysicsWorld3D.hpp"
#include "Physics/RigidBody3D.hpp"

#include "Resources/Resource.hpp"
#include "Resources/ResourceManager.hpp"
#include "Resources/Resources.hpp"

#include "Scene/Components/AudioListener.hpp"
#include "Scene/Components/AudioSource.hpp"
#include "Scene/Components/Camera.hpp"
#include "Scene/Components/Camera2D.hpp"
#include "Scene/Components/LightComponent.hpp"
#include "Scene/Components/MeshRenderer.hpp"
#include "Scene/Components/RectRenderer2D.hpp"
#include "Scene/Components/StaticSprite2D.hpp"
#include "Scene/Components/BoxCollider2D.hpp"
#include "Scene/Components/PhysicsWorld2D.hpp"
#include "Scene/Components/RigidBody2D.hpp"
#include "Scene/Components/BoxCollider3D.hpp"
#include "Scene/Components/PhysicsWorld3D.hpp"
#include "Scene/Components/RigidBody3D.hpp"
#include "Scene/Component.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Event.hpp"
#include "Scene/EventManager.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Transform.hpp"
#include "Scene/World.hpp"

#include "Script/ScriptModule.hpp"

#include "System/FileRecord.hpp"
#include "System/MemoryManager.hpp"
#include "System/System.hpp"

#include "UI/ImGUI.hpp"
#include "UI/ImGUIFlags.hpp"
#include "UI/ImGUIRenderer.hpp"
