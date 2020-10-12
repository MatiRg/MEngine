#include "Entity2D.hpp"
#include "Scene.hpp"
#include "World2D.hpp"
#include "../Core/Log.hpp"
#include "../Engine/Engine.hpp"
#include "../Core/XML.hpp"
#include "../System/MemoryManager.hpp"

CEntity2D::CEntity2D(CEngine* aEngine):
    Engine(aEngine),
    EventManager(aEngine->GetScene()->GetEventManager())
{
    Subscribe<EBeginFrame>(&CEntity2D::BeginFrame);
    Subscribe<EUpdate>(&CEntity2D::Update);
    Subscribe<ELateUpdate>(&CEntity2D::LateUpdate);
    Subscribe<EGUI>(&CEntity2D::GUI);
    Subscribe<ERender>(&CEntity2D::Render);
    Subscribe<EEndFrame>(&CEntity2D::EndFrame);
    //
    ADD_MEMORY_RECORD(this);
}

CEntity2D::~CEntity2D()
{
    ToEntitiesRemove = Children;
    DestroyChildrenRaw();
    //
    ToComponentsRemove = Components;
    DestroyComponentsRaw();
    UnSubscribe();
    //
    ERASE_MEMORY_RECORD(this);
}

bool CEntity2D::Load(CXMLElement* Root)
{
	ID = XML::LoadInt(Root, "ID", INVALID_ENTITY);
	if( ID == INVALID_ENTITY )
	{ 
		LOG(ESeverity::Error) << "Invalid Entity ID\n";
		return false;
	}

	Name = XML::LoadString(Root, "Name", "");
	Tag = XML::LoadString(Root, "Tag", "");

	CXMLElement* TransformElement = Root->GetElement("Transform2D");
	if (!TransformElement)
	{
		LOG(ESeverity::Error) << "No 'Transform2D' tag in Entity2D\n";
		return false;
	}
	if (!Transform.Load(TransformElement))
	{
		return false;
	}

	Updated = XML::LoadBool(Root, "Updated", true);
	Rendered = XML::LoadBool(Root, "Rendered", true);
	Started = XML::LoadBool(Root, "Started", false);

	if (!OnLoad(Root))
	{
		return false;
	}

	ComponentArray ToLoad = Components;
	XMLElementArray ComponentsElements = Root->GetElements("Component");
	for (auto i: ComponentsElements)
	{
		std::string Type = XML::LoadString(i, "Type", "");
		if (Type.empty())
		{
			LOG(ESeverity::Error) << "Invalid Field 'Type'\n";
			return false;
		}

		auto Found = std::find_if(ToLoad.begin(), ToLoad.end(), [&](IComponent* Component)
		{
			return Component->GetType() == Type;
		});
		if (Found != ToLoad.end())
		{
			auto Component = *Found;
			ToLoad.erase(Found);
			//
			if (!Component->OnLoad(i))
			{
				return false;
			}
		}
		else
		{
			auto Component = CreateComponent(Type);
			if (!Component)
			{
				return false;
			}
			if (!Component->OnLoad(i))
			{
				return false;
			}
		}
	}

	EntityArray ToLoadEntities = Children;
	XMLElementArray EntitiesElements = Root->GetElements("Entity2D");
	for (auto i : EntitiesElements)
	{
		std::string Type = XML::LoadString(i, "Type", "");
		if (Type.empty())
		{
			LOG(ESeverity::Error) << "Invalid Field 'Type'\n";
			return false;
		}

		auto Found = std::find_if(ToLoadEntities.begin(), ToLoadEntities.end(), [&](CEntity2D* Entity)
		{
			return Entity->GetType() == Type;
		});
		if (Found != ToLoadEntities.end())
		{
			auto Entity = *Found;
			ToLoadEntities.erase(Found);
			//
			if (!Entity->Load(i))
			{
				return false;
			}
		}
		else
		{
			auto Entity = CreateChild(Type);
			if (!Entity)
			{
				return false;
			}
			if (!Entity->Load(i))
			{
				return false;
			}
		}
	}

	return true;
}

bool CEntity2D::Save(CXMLElement* Root)
{
	XML::SaveInt(Root, "ID", ID);
	XML::SaveString(Root, "Name", Name);
	XML::SaveString(Root, "Tag", Tag);

	CXMLElement* TransformElement = Root->NewElement("Transform2D");
    if( !Transform.Save(TransformElement) )
    {
        return false;
    }

	XML::SaveBool(Root, "Updated", Updated);
	XML::SaveBool(Root, "Rendered", Rendered);
	XML::SaveBool(Root, "Started", Started);

    if( !OnSave(Root) )
    {
        return false;
    }
    
    for(const auto& i: Components)
    {
		CXMLElement* ComponentElement = Root->NewElement("Component");
		XML::SaveString(ComponentElement, "Type", i->GetType());

        if( !i->OnSave(ComponentElement) )
        {
            return false;
        }
    }

    for(const auto& i: Children)
    {
		CXMLElement* EntityElement = Root->NewElement("Entity2D");
		XML::SaveString(EntityElement, "Type", i->GetType());

        if( !i->Save(EntityElement) )
        {
            return false;
        }
    }

    return true;
}

void CEntity2D::SetUpdated(const bool Value)
{
    Updated = Value;
    if( Updated )
    {
        OnEnabled();
    }
    else
    {
        OnDisabled();
    }

    for(const auto& i: Components)
    {
        if( Updated )
        {
            i->OnEnabled();
        }
        else
        {
            i->OnDisabled();
        }
    }

    for(const auto& i: Children)
    {
        i->SetUpdated(Value);
    }
}

void CEntity2D::SetRendered(const bool Value)
{
    Rendered = Value;
    for(const auto& i: Children)
    {
        i->SetRendered(Value);
    }
}

void CEntity2D::SetParent(CEntity2D* NewParent) 
{
    if( Parent == NewParent || NewParent == this )
    {
        return;
    }
    auto RomovedThis = Parent ? Parent->RemoveChild(this) : this;
    if( NewParent )
    {
        NewParent->InsertChild(RomovedThis);
        if( NewParent->GetWorld() != GetWorld() )
        {
            if( GetWorld() )
            {
                GetWorld()->OnEntityDestroy(RomovedThis);
            }
            if( NewParent->GetWorld() )
            {
                NewParent->GetWorld()->OnEntityCreate(RomovedThis);
            }
        }
    }
    else
    {
        if( GetWorld() )
        {
            GetWorld()->OnEntityDestroy(RomovedThis);
        }
    }
    Transform.SetParent( NewParent ? &NewParent->GetTransform() : nullptr );
    Parent = NewParent; 
}

CEntity2D* CEntity2D::RemoveChild(CEntity2D* ToRemove)
{
    for(auto it = Children.begin(); it != Children.end(); ++it)
    {
        if( *it == ToRemove )
        {
            auto Child = *it;
            Children.erase(it);
            return Child;
        }
    }
    LOG(ESeverity::Warning) << "Entity2D - Removing Invalid Entity\n";
    return nullptr;
}

void CEntity2D::InsertChild(CEntity2D* NewChild)
{
    if( NewChild )
    {
        Children.push_back( NewChild );
    }
}

IComponent* CEntity2D::CreateComponent(const std::string& Type)
{
    auto Component = Engine->GetScene()->CreateComponent(Type);
    Components.push_back(Component);
    Component->SetOwner( this );
    Component->OnCreate();
    return Component;
}

bool CEntity2D::HasComponent(const std::string& Type) const
{
    auto Iterator = std::find_if(Components.begin(), Components.end(), [&](IComponent* Component)
    {
        return Component->GetType() == Type;
    });
    return Iterator != Components.end();
}

IComponent* CEntity2D::GetComponent(const std::string& Type) const
{
    auto Iterator = std::find_if(Components.begin(), Components.end(), [&](IComponent* Component)
    {
        return Component->GetType() == Type;
    });
    if( Iterator != Components.end() )
    {
        return *Iterator;
    }
    return nullptr;
}

ComponentArray CEntity2D::GetComponents(const std::string& Type) const
{
    ComponentArray Array;
    for(const auto& i: Components)
    {
        if( i->GetType() == Type )
        {
            Array.push_back( i );
        }
    }
    return Array;
}

void CEntity2D::DestroyComponentRaw(IComponent* Component)
{
    if( Component )
    {
        Component->OnDestroy();
        delete Component;
    }
}

void CEntity2D::DestroyComponentsRaw()
{
    for(const auto& i: ToComponentsRemove)
    {
        DestroyComponentRaw(i);
        auto Iterator = std::find_if(Components.begin(), Components.end(), [&](IComponent* Component)
        {
            return Component == i;
        });
        if( Iterator != Components.end() )
        {
            Components.erase(Iterator);
        }
    }
    ToComponentsRemove.clear();
}

void CEntity2D::Destroy(IComponent* Component)
{
    if( Component )
    {
        Component->GetOwner()->DestroyComponent(Component);
    }
}

void CEntity2D::DestroyComponent(IComponent* aComponent)
{
    auto Iterator = std::find_if(Components.begin(), Components.end(), [&](IComponent* Component)
    {
        return Component == aComponent;
    });
    if( Iterator != Components.end() )
    {
        ToComponentsRemove.push_back(*Iterator);
    }
}

void CEntity2D::DestroyComponent(const std::string& Type)
{
    auto Iterator = std::find_if(Components.begin(), Components.end(), [&](IComponent* Component)
    {
        return Component->GetType() == Type;
    });
    if( Iterator != Components.end() )
    {
        ToComponentsRemove.push_back(*Iterator);
    }
}

void CEntity2D::DestroyComponents(const std::string& Type)
{
    for(const auto& i: Components)
    {
        if( i->GetType() == Type )
        {
            ToComponentsRemove.push_back(i);
        }
    }
}

void CEntity2D::DestroyComponents() 
{ 
    for(const auto& i: Components)
    {
        ToComponentsRemove.push_back(i);
    }
}

CEntity2D* CEntity2D::CreateChild(const std::string& Type)
{
    auto Child = Engine->GetScene()->CreateEntity(Type);
    Child->SetParent( this ); // World Set Up
    Child->OnCreate();
    return Child;
}

CEntity2D* CEntity2D::GetChild(const int ID) const
{
    for(const auto& i: Children)
    {
        if( i->GetID() == ID )
        {
            return i;
        }
    }
    return nullptr;
}

CEntity2D* CEntity2D::GetChild(const std::string& Name) const
{
    for(const auto& i: Children)
    {
        if( i->GetName() == Name )
        {
            return i;
        }
    }
    return nullptr;
}

EntityArray CEntity2D::GetChildren(const std::string& Tag) const
{
    EntityArray Array;
    for(const auto& i: Children)
    {
        if( i->GetTag() == Tag )
        {
            Array.push_back(i);
        }
    }
    return Array;
}

void CEntity2D::DestroyEntityRaw(CEntity2D* Entity)
{
    if( Entity )
    {
        Entity->OnDestroy();
        if( Entity->IsAttachedToWorld() && Entity != Entity->GetWorld() )
        {
            Entity->GetWorld()->OnEntityDestroy(Entity);
        }
        delete Entity;
    }
}

void CEntity2D::DestroyChildrenRaw()
{
    for(const auto& i: ToEntitiesRemove)
    {
        DestroyEntityRaw(i);
        auto Iterator = std::find_if(Children.begin(), Children.end(), [&](CEntity2D* Entity)
        {
            return Entity == i;
        });
        if( Iterator != Children.end() )
        {
            Children.erase(Iterator);
        }
    }
    ToEntitiesRemove.clear();
}

void CEntity2D::Destroy()
{
    // Warning
    Destroy(this);
}

void CEntity2D::Destroy(CEntity2D* Entity)
{
    if( Entity )
    {
        if( Entity->HasParent() )
        {
            Entity->GetParent()->DestroyChild(Entity);
        }
        else
        {
            DestroyEntityRaw(Entity);
        }
    }
}

void CEntity2D::DestroyChild(CEntity2D* aEntity)
{
    auto Iterator = std::find_if(Children.begin(), Children.end(), [&](CEntity2D* Entity)
    {
        return Entity == aEntity;
    });
    if( Iterator != Children.end() )
    {
        ToEntitiesRemove.push_back(*Iterator);
    }
}

void CEntity2D::DestroyChild(const int ID)
{
    auto Iterator = std::find_if(Children.begin(), Children.end(), [&](CEntity2D* Entity)
    {
        return Entity->GetID() == ID;
    });
    if( Iterator != Children.end() )
    {
        ToEntitiesRemove.push_back(*Iterator);
    }
}

void CEntity2D::DestroyChild(const std::string& Name)
{
    auto Iterator = std::find_if(Children.begin(), Children.end(), [&](CEntity2D* Entity)
    {
        return Entity->GetName() == Name;
    });
    if( Iterator != Children.end() )
    {
        ToEntitiesRemove.push_back(*Iterator);
    }
}

void CEntity2D::DestroyChildren(const std::string& Tag)
{
    for(const auto& i: Children)
    {
        if( i->GetTag() == Tag )
        {
            ToEntitiesRemove.push_back(i);
        }
    }
}

void CEntity2D::DestroyChildren()
{
    for(const auto& i: Children)
    {
        ToEntitiesRemove.push_back(i);
    }
}

void CEntity2D::BeginFrame(const EBeginFrame&)
{
    if( !IsUpdated() )
    {
        return;
    }

    DestroyChildrenRaw();
    DestroyComponentsRaw();

    if( !Started )
    {
        Started = true;
        OnStart();
    }

    for(const auto& i: Components)
    {
        i->OnBeginFrame();
    }
    OnBeginFrame();
}

void CEntity2D::Update(const EUpdate& Event)
{
    if( !IsUpdated() )
    {
        return;
    }
    for(const auto& i: Components)
    {
        i->OnUpdate(Event.DT);
    }
    OnUpdate(Event.DT);
}

void CEntity2D::LateUpdate(const ELateUpdate& Event)
{
    if( !IsUpdated() )
    {
        return;
    }
    for(const auto& i: Components)
    {
        i->OnLateUpdate(Event.DT);
    }
    OnLateUpdate(Event.DT);
}

void CEntity2D::GUI(const EGUI&)
{
    if( !IsUpdated() )
    {
        return;
    }
    for(const auto& i: Components)
    {
        i->OnGUI();
    }
    OnGUI();
}

void CEntity2D::Render(const ERender&)
{
    if( !IsRendered() )
    {
        return;
    }
    for(const auto& i: Components)
    {
        i->OnRender();
    }
    OnRender();
}

void CEntity2D::EndFrame(const EEndFrame&)
{
    if( !IsUpdated() )
    {
        return;
    }
    for(const auto& i: Components)
    {
        i->OnEndFrame();
    }
    OnEndFrame();
}

void CEntity2D::UnSubscribe()
{
    EventManager->UnSubscribe(this);
}
