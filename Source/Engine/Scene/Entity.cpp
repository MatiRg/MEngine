#include "Entity.hpp"
#include "Scene.hpp"
#include "World.hpp"
#include "../Core/Log.hpp"
#include "../Engine/Engine.hpp"
#include "../Core/XML.hpp"
#include "../System/MemoryManager.hpp"

CEntity::CEntity(CEngine* aEngine) :
    Engine(aEngine),
    EventManager(aEngine->GetScene()->GetEventManager())
{
    Subscribe<EBeginFrame>(&CEntity::BeginFrame);
    Subscribe<EUpdate>(&CEntity::Update);
    Subscribe<ELateUpdate>(&CEntity::LateUpdate);
    Subscribe<EGUI>(&CEntity::GUI);
    Subscribe<ERender>(&CEntity::Render);
    Subscribe<EEndFrame>(&CEntity::EndFrame);
    //
    ADD_MEMORY_RECORD(this);
}

CEntity::~CEntity()
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

bool CEntity::Load(CXMLElement* Root)
{
    ID = XML::LoadInt(Root, "ID", INVALID_ENTITY);
    if (ID == INVALID_ENTITY)
    {
        LOG(ESeverity::Error) << "Invalid Entity ID\n";
        return false;
    }

    Name = XML::LoadString(Root, "Name", "");
    Tag = XML::LoadString(Root, "Tag", "");

    CXMLElement* TransformElement = Root->GetElement("Transform");
    if (!TransformElement)
    {
        LOG(ESeverity::Error) << "No 'Transform' tag in Entity\n";
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
    for (auto i : ComponentsElements)
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
    XMLElementArray EntitiesElements = Root->GetElements("Entity");
    for (auto i : EntitiesElements)
    {
        std::string Type = XML::LoadString(i, "Type", "");
        if (Type.empty())
        {
            LOG(ESeverity::Error) << "Invalid Field 'Type'\n";
            return false;
        }

        auto Found = std::find_if(ToLoadEntities.begin(), ToLoadEntities.end(), [&](CEntity* Entity)
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

bool CEntity::Save(CXMLElement* Root)
{
    XML::SaveInt(Root, "ID", ID);
    XML::SaveString(Root, "Name", Name);
    XML::SaveString(Root, "Tag", Tag);

    CXMLElement* TransformElement = Root->NewElement("Transform");
    if (!Transform.Save(TransformElement))
    {
        return false;
    }

    XML::SaveBool(Root, "Updated", Updated);
    XML::SaveBool(Root, "Rendered", Rendered);
    XML::SaveBool(Root, "Started", Started);

    if (!OnSave(Root))
    {
        return false;
    }

    for (const auto& i : Components)
    {
        CXMLElement* ComponentElement = Root->NewElement("Component");
        XML::SaveString(ComponentElement, "Type", i->GetType());

        if (!i->OnSave(ComponentElement))
        {
            return false;
        }
    }

    for (const auto& i : Children)
    {
        CXMLElement* EntityElement = Root->NewElement("Entity");
        XML::SaveString(EntityElement, "Type", i->GetType());

        if (!i->Save(EntityElement))
        {
            return false;
        }
    }

    return true;
}

void CEntity::SetUpdated(const bool Value)
{
    Updated = Value;
    if (Updated)
    {
        OnEnabled();
    }
    else
    {
        OnDisabled();
    }

    for (const auto& i : Components)
    {
        if (Updated)
        {
            i->OnEnabled();
        }
        else
        {
            i->OnDisabled();
        }
    }

    for (const auto& i : Children)
    {
        i->SetUpdated(Value);
    }
}

void CEntity::SetRendered(const bool Value)
{
    Rendered = Value;
    for (const auto& i : Children)
    {
        i->SetRendered(Value);
    }
}

void CEntity::SetParent(CEntity* NewParent)
{
    if (Parent == NewParent || NewParent == this)
    {
        return;
    }
    auto RomovedThis = Parent ? Parent->RemoveChild(this) : this;
    if (NewParent)
    {
        NewParent->InsertChild(RomovedThis);
        if (NewParent->GetWorld() != GetWorld())
        {
            if (GetWorld())
            {
                GetWorld()->OnEntityDestroy(RomovedThis);
            }
            if (NewParent->GetWorld())
            {
                NewParent->GetWorld()->OnEntityCreate(RomovedThis);
            }
        }
    }
    else
    {
        if (GetWorld())
        {
            GetWorld()->OnEntityDestroy(RomovedThis);
        }
    }
    Transform.SetParent(NewParent ? &NewParent->GetTransform() : nullptr);
    Parent = NewParent;
}

CEntity* CEntity::RemoveChild(CEntity* ToRemove)
{
    for (auto it = Children.begin(); it != Children.end(); ++it)
    {
        if (*it == ToRemove)
        {
            auto Child = *it;
            Children.erase(it);
            return Child;
        }
    }
    LOG(ESeverity::Warning) << "Entity - Removing Invalid Entity\n";
    return nullptr;
}

void CEntity::InsertChild(CEntity* NewChild)
{
    if (NewChild)
    {
        Children.push_back(NewChild);
    }
}

IComponent* CEntity::CreateComponent(const std::string& Type)
{
    auto Component = Engine->GetScene()->CreateComponent(Type);
    Components.push_back(Component);
    Component->SetOwner(this);
    Component->OnCreate();
    return Component;
}

bool CEntity::HasComponent(const std::string& Type) const
{
    auto Iterator = std::find_if(Components.begin(), Components.end(), [&](IComponent* Component)
        {
            return Component->GetType() == Type;
        });
    return Iterator != Components.end();
}

IComponent* CEntity::GetComponent(const std::string& Type) const
{
    auto Iterator = std::find_if(Components.begin(), Components.end(), [&](IComponent* Component)
        {
            return Component->GetType() == Type;
        });
    if (Iterator != Components.end())
    {
        return *Iterator;
    }
    return nullptr;
}

ComponentArray CEntity::GetComponents(const std::string& Type) const
{
    ComponentArray Array;
    for (const auto& i : Components)
    {
        if (i->GetType() == Type)
        {
            Array.push_back(i);
        }
    }
    return Array;
}

void CEntity::DestroyComponentRaw(IComponent* Component)
{
    if (Component)
    {
        Component->OnDestroy();
        delete Component;
    }
}

void CEntity::DestroyComponentsRaw()
{
    for (const auto& i : ToComponentsRemove)
    {
        DestroyComponentRaw(i);
        auto Iterator = std::find_if(Components.begin(), Components.end(), [&](IComponent* Component)
            {
                return Component == i;
            });
        if (Iterator != Components.end())
        {
            Components.erase(Iterator);
        }
    }
    ToComponentsRemove.clear();
}

void CEntity::Destroy(IComponent* Component)
{
    if (Component)
    {
        Component->GetOwner()->DestroyComponent(Component);
    }
}

void CEntity::DestroyComponent(IComponent* aComponent)
{
    auto Iterator = std::find_if(Components.begin(), Components.end(), [&](IComponent* Component)
        {
            return Component == aComponent;
        });
    if (Iterator != Components.end())
    {
        ToComponentsRemove.push_back(*Iterator);
    }
}

void CEntity::DestroyComponent(const std::string& Type)
{
    auto Iterator = std::find_if(Components.begin(), Components.end(), [&](IComponent* Component)
        {
            return Component->GetType() == Type;
        });
    if (Iterator != Components.end())
    {
        ToComponentsRemove.push_back(*Iterator);
    }
}

void CEntity::DestroyComponents(const std::string& Type)
{
    for (const auto& i : Components)
    {
        if (i->GetType() == Type)
        {
            ToComponentsRemove.push_back(i);
        }
    }
}

void CEntity::DestroyComponents()
{
    for (const auto& i : Components)
    {
        ToComponentsRemove.push_back(i);
    }
}

CEntity* CEntity::CreateChild(const std::string& Type)
{
    auto Child = Engine->GetScene()->CreateEntity(Type);
    Child->SetParent(this); // World Set Up
    Child->OnCreate();
    return Child;
}

CEntity* CEntity::GetChild(const int ID) const
{
    for (const auto& i : Children)
    {
        if (i->GetID() == ID)
        {
            return i;
        }
    }
    return nullptr;
}

CEntity* CEntity::GetChild(const std::string& Name) const
{
    for (const auto& i : Children)
    {
        if (i->GetName() == Name)
        {
            return i;
        }
    }
    return nullptr;
}

EntityArray CEntity::GetChildren(const std::string& Tag) const
{
    EntityArray Array;
    for (const auto& i : Children)
    {
        if (i->GetTag() == Tag)
        {
            Array.push_back(i);
        }
    }
    return Array;
}

void CEntity::DestroyEntityRaw(CEntity* Entity)
{
    if (Entity)
    {
        Entity->OnDestroy();
        if (Entity->IsAttachedToWorld() && Entity != Entity->GetWorld())
        {
            Entity->GetWorld()->OnEntityDestroy(Entity);
        }
        delete Entity;
    }
}

void CEntity::DestroyChildrenRaw()
{
    for (const auto& i : ToEntitiesRemove)
    {
        DestroyEntityRaw(i);
        auto Iterator = std::find_if(Children.begin(), Children.end(), [&](CEntity* Entity)
            {
                return Entity == i;
            });
        if (Iterator != Children.end())
        {
            Children.erase(Iterator);
        }
    }
    ToEntitiesRemove.clear();
}

void CEntity::Destroy()
{
    // Warning
    Destroy(this);
}

void CEntity::Destroy(CEntity* Entity)
{
    if (Entity)
    {
        if (Entity->HasParent())
        {
            Entity->GetParent()->DestroyChild(Entity);
        }
        else
        {
            DestroyEntityRaw(Entity);
        }
    }
}

void CEntity::DestroyChild(CEntity* aEntity)
{
    auto Iterator = std::find_if(Children.begin(), Children.end(), [&](CEntity* Entity)
        {
            return Entity == aEntity;
        });
    if (Iterator != Children.end())
    {
        ToEntitiesRemove.push_back(*Iterator);
    }
}

void CEntity::DestroyChild(const int ID)
{
    auto Iterator = std::find_if(Children.begin(), Children.end(), [&](CEntity* Entity)
        {
            return Entity->GetID() == ID;
        });
    if (Iterator != Children.end())
    {
        ToEntitiesRemove.push_back(*Iterator);
    }
}

void CEntity::DestroyChild(const std::string& Name)
{
    auto Iterator = std::find_if(Children.begin(), Children.end(), [&](CEntity* Entity)
        {
            return Entity->GetName() == Name;
        });
    if (Iterator != Children.end())
    {
        ToEntitiesRemove.push_back(*Iterator);
    }
}

void CEntity::DestroyChildren(const std::string& Tag)
{
    for (const auto& i : Children)
    {
        if (i->GetTag() == Tag)
        {
            ToEntitiesRemove.push_back(i);
        }
    }
}

void CEntity::DestroyChildren()
{
    for (const auto& i : Children)
    {
        ToEntitiesRemove.push_back(i);
    }
}

void CEntity::BeginFrame(const EBeginFrame&)
{
    if (!IsUpdated())
    {
        return;
    }

    DestroyChildrenRaw();
    DestroyComponentsRaw();

    if (!Started)
    {
        Started = true;
        OnStart();
    }

    for (const auto& i : Components)
    {
        i->OnBeginFrame();
    }
    OnBeginFrame();
}

void CEntity::Update(const EUpdate& Event)
{
    if (!IsUpdated())
    {
        return;
    }
    for (const auto& i : Components)
    {
        i->OnUpdate(Event.DT);
    }
    OnUpdate(Event.DT);
}

void CEntity::LateUpdate(const ELateUpdate& Event)
{
    if (!IsUpdated())
    {
        return;
    }
    for (const auto& i : Components)
    {
        i->OnLateUpdate(Event.DT);
    }
    OnLateUpdate(Event.DT);
}

void CEntity::GUI(const EGUI&)
{
    if (!IsUpdated())
    {
        return;
    }
    for (const auto& i : Components)
    {
        i->OnGUI();
    }
    OnGUI();
}

void CEntity::Render(const ERender&)
{
    if (!IsRendered())
    {
        return;
    }
    for (const auto& i : Components)
    {
        i->OnRender();
    }
    OnRender();
}

void CEntity::EndFrame(const EEndFrame&)
{
    if (!IsUpdated())
    {
        return;
    }
    for (const auto& i : Components)
    {
        i->OnEndFrame();
    }
    OnEndFrame();
}

void CEntity::UnSubscribe()
{
    EventManager->UnSubscribe(this);
}
