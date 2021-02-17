#include "Level2.hpp"
#include "MainApp.hpp"
#include "Globals.hpp"

static constexpr char ITEM_TAG[] = "Item";

//

CGameObject::CGameObject(CEngine* aEngine) :
	CEntity(aEngine)
{
}

CGameObject::~CGameObject()
{
}

//

CPlayerObject::CPlayerObject(CEngine* aEngine) :
	CGameObject(aEngine)
{
}

CPlayerObject::~CPlayerObject()
{
}

void CPlayerObject::OnStart()
{
	TotalTime = 0.0f;
	Score = 0;
}

void CPlayerObject::OnUpdate(const float DT)
{
	TotalTime += DT;
	//
	Vector3 Position = Transform.GetWorldPosition();
	float D = Math::Distance(Goal, Position);
}

void CPlayerObject::OnRender()
{
	CDebugDrawer* Drawer = Engine->GetDebugDrawer();
	Drawer->AddLine(Transform.GetWorldPosition(), Transform.GetWorldPosition()-Transform.GetForward()*1.0f, Color::BLUE); // ?
}

void CPlayerObject::OnGUI()
{
	Vector3 Position = Transform.GetWorldPosition();
	float D = Math::Distance(Goal, Position);
	//
	CImGUI* UI = Engine->GetImGUI();
	UI->Begin("GUI");
	UI->Text("Time: "+std::to_string(TotalTime));
	UI->Text("Score: " + std::to_string(Score));
	UI->Separator();
	UI->Text("Distance: " + std::to_string(D));
	UI->End();
}

void CPlayerObject::OnCollisionEnter(const SEntityCollision3D& Collision)
{
	if (Collision.Entity->GetTag() == ITEM_TAG)
	{
		CEntity::Destroy(Collision.Entity);
		//Collision.Entity->SetActive(false);
		++Score;
	}
}

void CPlayerObject::OnCollisionStay(const SEntityCollision3D& Collision)
{
}

void CPlayerObject::OnCollisionLeave(const SEntityCollision3D& Collision)
{
}

//

enum class ERotationAxis
{
	X,
	Y,
	Z
};

class CRotator: public IComponent
{
public:
	CRotator(CEngine* aEngine): 
		IComponent(aEngine) 
	{
	}

	COMPONENT(CRotator)

	void OnUpdate(const float DT) override
	{
		CTransform& Transform = GetOwner()->GetTransform();
		switch (Axis)
		{
		case ERotationAxis::X:
			Transform.Rotate(Speed * DT, 0.0f, 0.0f);
			break;
		case ERotationAxis::Y:
			Transform.Rotate(0.0f, Speed * DT, 0.0f);
			break;
		case ERotationAxis::Z:
			Transform.Rotate(0.0f, 0.0f, Speed * DT);
			break;
		default:
			break;
		}
	}

	void SetAxis(const ERotationAxis aAxis) { Axis = aAxis; }
	ERotationAxis GetAxis() const { return Axis; }
	
	void SetSpeed(const float aSpeed) { Speed = aSpeed; }
	float GetSpeed() const { return Speed; }
private:
	ERotationAxis Axis = ERotationAxis::X;
	float Speed = 1.0f;
};

//

CLevel2::CLevel2(CMainApp* aApp) :
	IUpdatable("Level2"),
	App(aApp)
{
}

CLevel2::~CLevel2()
{
}

void CLevel2::LoadMap()
{
	std::string Path;
	if (!App->GetResources()->FindPath("Labirynt.xml", Path))
	{
		return;
	}

	CXMLDocument Doc;
	if (!Doc.Load(Path))
	{
		LOG(ESeverity::Error) << "Invalid Labyrinth Map\n";
		return;
	}

	CXMLElement* Root = Doc.GetElement("map");
	if (!Root)
	{
		LOG(ESeverity::Error) << "No 'map' Element\n";
		return;
	}
	Width = Root->GetIntAttribute("W", 0);
	Height = Root->GetIntAttribute("H", 0);

	XMLElementArray Rows = Root->GetElements("row");
	for (const auto& i : Rows)
	{
		int Row = i->GetIntAttribute("id", 0);
		XMLElementArray Cols = i->GetElements("col");
		for (const auto& j : Cols)
		{
			int Col = j->GetIntAttribute("id", 0);
			int Value = j->GetInt(0);
			CGameObject* Object = World->CreateModel<CGameObject>("cube.dae", Materials[Math::Random(0, 4)].get());
			Object->GetTransform().SetPosition({ static_cast<float>(Row), static_cast<float>(Value == 2 ? 0 : Value), static_cast<float>(Col) });
			CBoxCollider3D* ColliderWall = Object->CreateComponent<CBoxCollider3D>();
			CRigidBody3D* WallBody = Object->CreateComponent<CRigidBody3D>();
			WallBody->SetBodyType(ERigidBodyType3D::Static);
			//
			Object->SetValue(Value);
			//
			if (Value == 2)
			{
				CGameObject* Item = World->CreateModel<CGameObject>("cube.dae", Materials[5].get());
				Item->GetTransform().SetPosition({ static_cast<float>(Row), 0.75f, static_cast<float>(Col) });
				Item->GetTransform().SetScale(0.5f);
				//
				CBoxCollider3D* ColliderItem = Item->CreateComponent<CBoxCollider3D>();
				ColliderItem->SetSize(Vector3(0.25f));
				CRigidBody3D* ItemBody = Item->CreateComponent<CRigidBody3D>();
				ItemBody->SetBodyType(ERigidBodyType3D::Static);
				CRotator* ItemRotator = Item->CreateComponent<CRotator>();
				ItemRotator->SetSpeed(16.0f);
				ItemRotator->SetAxis(ERotationAxis::Y);

				Item->SetValue(Value);
				Item->SetTag(ITEM_TAG);
			}
		}
	}

	CXMLElement* StartXRoot = Root->GetElement("start_x");
	if (!StartXRoot)
	{
		LOG(ESeverity::Error) << "No Start X\n";
		return;
	}
	StartX = StartXRoot->GetInt(0);
	//
	CXMLElement* StartYRoot = Root->GetElement("start_y");
	if (!StartYRoot)
	{
		LOG(ESeverity::Error) << "No Start Y\n";
		return;
	}
	StartY = StartYRoot->GetInt(0);

	Vector3 Goal = Vector3::ZERO;
	CXMLElement* EndXRoot = Root->GetElement("end_x");
	if (!EndXRoot)
	{
		LOG(ESeverity::Error) << "No End X\n";
		return;
	}
	Goal.x = EndXRoot->GetInt(0);
	//
	CXMLElement* EndYRoot = Root->GetElement("end_y");
	if (!EndYRoot)
	{
		LOG(ESeverity::Error) << "No End Y\n";
		return;
	}
	Goal.z = EndYRoot->GetInt(0);

	// Player
	PlayerObject = World->CreateModel<CPlayerObject>("cube.dae");
	PlayerObject->GetTransform().SetPosition({ static_cast<float>(StartX), 0.85f, static_cast<float>(StartY) });
	PlayerObject->SetName("Player");
	PlayerObject->GetTransform().SetScale(0.5f);
	PlayerObject->SetGoal(Goal);
	//
	CBoxCollider3D* PlayerCollider = PlayerObject->CreateComponent<CBoxCollider3D>();
	PlayerCollider->SetSize(Vector3(0.25f));
	PlayerBody = PlayerObject->CreateComponent<CRigidBody3D>();
	PlayerBody->SetBodyType(ERigidBodyType3D::Dynamic);
	PlayerBody->SetAngularFactor(Vector3::ZERO);
}

void CLevel2::OnInit()
{
	App->GetScene()->RegisterComponent<CRotator>();
	App->GetScene()->RegisterEntity<CGameObject>();
	App->GetScene()->RegisterEntity<CPlayerObject>();
	//
	CResources* Resources = App->GetResources();
	//
	IShader* ShaderCube = Resources->CreateResource<IShader>("TextureLSDEffects.shader");
	ITexture2D* TextureCube = Resources->CreateResource<ITexture2D>("wall.jpg");
	ITexture2D* TextureItem = Resources->CreateResource<ITexture2D>("item.jpg");
	//
	Materials[0] = std::make_unique<CMaterial>();
	Materials[0]->SetShader(ShaderCube);
	Materials[0]->SetTexture("Diffuse", TextureCube);
	Materials[0]->SetInt("SwitchVar", 0);
	//
	Materials[1] = std::make_unique<CMaterial>();
	Materials[1]->SetShader(ShaderCube);
	Materials[1]->SetTexture("Diffuse", TextureCube);
	Materials[1]->SetInt("SwitchVar", 1);
	//
	Materials[2] = std::make_unique<CMaterial>();
	Materials[2]->SetShader(ShaderCube);
	Materials[2]->SetTexture("Diffuse", TextureCube);
	Materials[2]->SetInt("SwitchVar", 2);
	//
	Materials[3] = std::make_unique<CMaterial>();
	Materials[3]->SetShader(ShaderCube);
	Materials[3]->SetTexture("Diffuse", TextureCube);
	Materials[3]->SetInt("SwitchVar", 3);
	//
	Materials[4] = std::make_unique<CMaterial>();
	Materials[4]->SetShader(ShaderCube);
	Materials[4]->SetTexture("Diffuse", TextureCube);
	Materials[4]->SetInt("SwitchVar", 4);

	Materials[5] = std::make_unique<CMaterial>();
	Materials[5]->SetShader(ShaderCube);
	Materials[5]->SetTexture("Diffuse", TextureItem);
	Materials[5]->SetInt("SwitchVar", 4);
	//
	BlurEffect = App->GetRenderer3D()->CreatePostEffect("ScreenBlurBox.shader", 10);
	if (BlurEffect)
	{
		BlurEffect->SetEnabled(false);
		BlurEffect->SetInt("Samples", 1);
	}
	// App->GetRenderer3D()->SetMSAASamples(8.0f);
}

namespace
{
	void PrintModel(CMeshNode* Node, int Level)
	{
		if (Node)
		{
			std::string Margin(Level, ' ');
			LOG(ESeverity::Info) << Margin << "Node: " << Node->GetName() << "\n";

			for (const auto& i : Node->GetMeshes())
			{
				LOG(ESeverity::Info) << Margin << "+Mesh: " << i->GetName() << "\n";
			}

			for (const auto& i : Node->GetChildren())
			{
				PrintModel(i, Level + 1);
			}
		}
	}
}

void CLevel2::OnEnter()
{
	App->GetInput()->SetMouseMode(EMouseMode::Relative);
	//
	World = std::make_unique<CWorld>(App->GetEngine());
	CPhysicsWorld3D* PhysicsWorld = World->CreateComponent<CPhysicsWorld3D>();
	PhysicsWorld->SetGravity({ 0.0f, -10.0f, 0.0f });
	PhysicsWorld->SetDebugDraw(true);

	// Create Sky Dome
	CEntity* SkyDome = World->CreateModel<CEntity>("sky2.dae");
	// Make it Big
	SkyDome->GetTransform().SetScale(Vector3(500.0f));

	CEntity* Sun = World->CreateChild<CEntity>();
	//
	CLightComponent* LightSun = Sun->CreateComponent<CLightComponent>();
	LightSun->SetLightType(ELightType::Direction);
	LightSun->SetTemperature(5500.f);
	//
	Sun->GetTransform().SetRotation(Quaternion(-30.0f, 0.0f, 0.0f));

	CameraObject = World->CreateChild<CEntity>();
	CameraTransform = &CameraObject->GetTransform();
	CameraTransform->SetRotation(Quaternion(29.0f, 0.05f, 0.0f));
	CameraTransform->SetPosition(Vector3(5.25f, 14.15f, -3.5f));
	//
	CameraComponent = CameraObject->CreateComponent<CCamera>();
	CameraComponent->SetAspect(App->GetWindow()->GetAspectRatio());
	CameraComponent->SetFOV(Fov);
	CameraComponent->SetNearClipPlane(0.3f);
	CameraComponent->SetFarClipPlane(1000.0f);

	LoadMap();
}

void CLevel2::OnUpdate(const float TimeStep)
{
	IInput* Input = App->GetInput();
	// Exit
	if (Input->IsKeyDown(EKey::Escape))
	{
		App->Quit();
	}
	// Camera
	Vector3 CameraPosition = CameraTransform->GetPosition();
	if (Input->IsKeyPressed(EKey::W))
	{
		CameraPosition += Vector3::FORWARD * CameraSpeed * TimeStep;
	}
	if (Input->IsKeyPressed(EKey::S))
	{
		CameraPosition += Vector3::BACK * CameraSpeed * TimeStep;
	}
	if (Input->IsKeyPressed(EKey::A))
	{
		CameraPosition += Vector3::LEFT * CameraSpeed * TimeStep;
	}
	if (Input->IsKeyPressed(EKey::D))
	{
		CameraPosition += Vector3::RIGHT * CameraSpeed * TimeStep;
	}
	CameraTransform->SetPosition(CameraPosition);

	ScrollSpeed = Input->GetMouseWheel().y;
	Fov -= ScrollSpeed;
	if (Fov <= 25.0f)
		Fov = 25.0f;
	if (Fov >= 45.0f)
		Fov = 45.0f;
	ScrollSpeed = 0.0f;
	CameraComponent->SetFOV(Fov);

	// Player Controls
	Vector3 PlayerVelocity = Vector3::ZERO;
	if (Input->IsKeyPressed(EKey::Up))
	{
		PlayerVelocity.z = PlayerSpeed;
	}
	if (Input->IsKeyPressed(EKey::Down))
	{
		PlayerVelocity.z = -PlayerSpeed;
	}
	if (Input->IsKeyPressed(EKey::Left))
	{
		PlayerVelocity.x = -PlayerSpeed;
	}
	if (Input->IsKeyPressed(EKey::Right))
	{
		PlayerVelocity.x = PlayerSpeed;
	}
	PlayerBody->SetLinearVelocity(PlayerVelocity);

	// Blur Toggle
	if (Input->IsKeyDown(EKey::V))
	{
		BlurEffect->SetEnabled(!BlurEffect->IsEnabled());
	}
	// Wireframe Mode Toggle
	if (Input->IsKeyDown(EKey::B))
	{
		App->GetRenderer3D()->SetWireframe(!App->GetRenderer3D()->GetWireframe());
	}
}

void CLevel2::OnLateUpdate(const float)
{
}

void CLevel2::OnGUI()
{
	/*CImGUI* Im = App->GetImGUI();
	Im->Begin("Level Demo");
	Im->Text("Position: [" + std::to_string(CameraTransform->GetPosition().x) + ", " + std::to_string(CameraTransform->GetPosition().y) + ", " + std::to_string(CameraTransform->GetPosition().z) + "]");
	Im->Text("Aspect: " + std::to_string(CameraComponent->GetAspect()));
	Im->Text("Yaw: " + std::to_string(Yaw));
	Im->Text("Pitch: " + std::to_string(Pitch));
	Im->Text("FOV: " + std::to_string(Fov));
	Im->End();*/
}

void CLevel2::OnRender()
{
}

void CLevel2::OnLeave()
{
	World.reset();
	MaterialArray{}.swap(Materials);
	//
	App->GetInput()->SetMouseMode(EMouseMode::Normal);
}

void CLevel2::OnExit()
{
}