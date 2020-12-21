#pragma once
#include "../Core/Log.hpp"
#include "../Core/NonCopyable.hpp"
#include <functional>
#include <list>
#include <unordered_map>
#include <algorithm>

class CEntity;

class CEventManager final
{
    class IGameEvent: public NonCopyableMovable
    {
    public:
        IGameEvent() = default;
        virtual ~IGameEvent() = default;
    protected:
        static int GetNextFamily()
        {
            static int NextFamily = 0;
            return NextFamily++;
        }
    };

    template<class T>
    class TGameEvent final: public IGameEvent
    {
    private:
        using EventType = T;
    public:
        TGameEvent(const EventType& aData): Data(aData) {}
        ~TGameEvent() = default;

        const EventType& GetData() const { return Data; }

        static int GetFamily()
        {
            static int Family = IGameEvent::GetNextFamily();
            return Family;
        }
    private:
        const EventType& Data;
    };

    template<class E>
    using Function = std::function<void(const E&)>;
    using FunctionData = std::tuple<CEntity*, Function<IGameEvent>>;
    using FunctionContainer = std::list<FunctionData>;

    template<class E>
    class TCallBack final
    {
    private:
        using EventType = E;
    public:
        TCallBack(const Function<EventType>& aFunction): Function( aFunction ) {}

        void operator()(const IGameEvent& Event)
        {
            Function( static_cast<const TGameEvent<EventType>&>(Event).GetData() );
        }

        Function<EventType> Function;
    };
public:
    CEventManager() = default;
    ~CEventManager() = default;

    template<class E, class F>
    void Subscribe(CEntity* Entity, const F& aFunction)
    {
        auto& Container = EventReceivers[TGameEvent<E>::GetFamily()];
        for(auto it = Container.begin(); it != Container.end(); ++it)
        {
            if( std::get<0>(*it) == Entity )
            {
                LOG(ESeverity::Warning) << "Object Already Registered for this Event\n";
                return;
            }
        }
        Container.push_back( {Entity, TCallBack<E>(std::bind( aFunction, Entity, std::placeholders::_1 ))} );
    }

    template<class E, class...Args>
    void Send(Args&&...args)
    {
        E EventData( std::forward<Args>(args)... );

        auto& Container = EventReceivers[TGameEvent<E>::GetFamily()];
        for(auto it = Container.begin(); it != Container.end(); ++it)
        {
            TGameEvent<E> Event( EventData );
            std::get<1>(*it)( Event );
        }
    }

    void UnSubscribe(CEntity* Entity)
    {
        for(auto& i: EventReceivers)
        {
            auto& Container = i.second;

            Container.erase( std::remove_if( Container.begin(), Container.end(), [&](const FunctionData& Data)
            {
                return std::get<0>(Data) == Entity;
            }), Container.end() );
        }
    }
private:
    std::unordered_map<int, FunctionContainer> EventReceivers; // EventClass, Container(Entity, Function)
};
