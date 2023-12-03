#ifndef dge_event_bus_hpp
#define dge_event_bus_hpp

#include "../ecs/ecs.hpp"
#include "event.hpp"
#include <map>
#include <typeindex>
#include <memory>
#include <list>

namespace dge
{

class i_event_callback
{
public:

    virtual ~i_event_callback() = default;
    
    void execute(event& e)
    {
        call(e);
    }

private:

    virtual void call(event& e) = 0;
  
};

template<typename OwnerType, typename EventType>
class event_callback : public i_event_callback
{
public:

    using callback_function = void (OwnerType::*)(EventType&);

    event_callback(OwnerType* owner, callback_function callback)
    : m_owner(owner), m_callback(callback)
    {
    }

    virtual ~event_callback() override = default;

private:

    OwnerType* m_owner;
    callback_function m_callback;

    virtual void call(event& e) override
    {
        std::invoke(m_callback, m_owner, static_cast<EventType&>(e));
    }

};

class event_bus
{
public:

    event_bus()
    {

    }

    ~event_bus()
    {

    }

    void reset()
    {
        m_subscribers.clear();
    }

    template<typename OwnerType, typename EventType>
    void subscribe_to_event(OwnerType* owner, void (OwnerType::*callback)(EventType&))
    {
        if (m_subscribers[typeid(EventType)].get() == nullptr) {
            m_subscribers[typeid(EventType)] = std::make_unique<handler_list>();
        }
        auto subscriber = std::make_unique<event_callback<OwnerType, EventType>>(owner, callback);
        m_subscribers[typeid(EventType)]->push_back(std::move(subscriber));
    }

    template<typename EventType, typename ...Args>
    void emit_event(Args&& ...args)
    {
        auto handlers = m_subscribers[typeid(EventType)].get();
        if (handlers) {
            for (auto it = handlers->begin(); it != handlers->end(); ++it) {
                auto handler = it->get();
                EventType event(std::forward<Args>(args)...);
                handler->execute(event);
            }
        }
    }

private:
    
    using handler_list = std::list<std::unique_ptr<i_event_callback>>;

    std::map<std::type_index, std::unique_ptr<handler_list>> m_subscribers;

};

}

#endif