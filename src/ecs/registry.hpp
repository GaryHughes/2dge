#ifndef dge_ecs_registry_hpp
#define dge_ecs_registry_hpp

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include "entity.hpp"
#include "pool.hpp"
#include "system.hpp"
#include "../logger.hpp"

namespace dge
{
namespace ecs
{

class registry
{
public:

    entity create_entity();
    void destroy_entity(entity& e);

    template<typename ComponentType, typename ... TArgs>
    void add_component(const entity& e, TArgs&& ... args)
    {
        const auto component_id = component<ComponentType>::id();
        const auto entity_id = e.id();

        if (component_id >= m_component_pools.size()) {
            m_component_pools.resize(component_id + 1,  nullptr);
        }

        if (m_component_pools[component_id] == nullptr) {
            auto p = std::make_shared<pool<ComponentType>>();
            m_component_pools[component_id] = p;
        }

        std::shared_ptr<pool<ComponentType>> p = std::static_pointer_cast<pool<ComponentType>>(m_component_pools[component_id]);
    
        if (entity_id >= p->size()) {
            p->resize(m_entity_count);
        }

        ComponentType c(std::forward<TArgs>(args)...);

        p->set(entity_id, c);

        m_entity_component_signatures[entity_id].set(component_id);

        logger::info("component id = " + std::to_string(component_id) + " was added to entity id = " + std::to_string(entity_id));
    }

    template<typename ComponentType>
    void remove_component(const entity& e)
    {
        const auto component_id = component<ComponentType>::id();
        const auto entity_id = e.id();
        // TODO - range check
        m_entity_component_signatures[entity_id].set(component_id, false);
        logger::info("component id = " + std::to_string(component_id) + " was removed from entity id = " + std::to_string(entity_id));
    }

    template<typename ComponentType>
    bool has_component(const entity& e)
    {
        const auto component_id = component<ComponentType>::id();
        const auto entity_id = e.id();
        // TODO - range check
        return m_entity_component_signatures[entity_id].test(component_id);
    }

    template<typename ComponentType>
    ComponentType& get_component(const entity& e)
    {
        const auto component_id = component<ComponentType>::id();
        const auto entity_id = e.id();
        std::shared_ptr<pool<ComponentType>> component_pool = std::static_pointer_cast<pool<ComponentType>>(m_component_pools[component_id]);
        return component_pool[entity_id];
    }

    template<typename SystemType, typename ... TArgs>
    void add_system(TArgs&& ... args)
    {
        system_ptr new_system = std::make_shared<SystemType>(std::forward<TArgs>(args)...);
        m_systems.insert(std::make_pair(std::type_index(typeid(SystemType)), new_system));
    }

    template<typename SystemType>
    void remove_system()
    {
        auto system = m_systems.find(std::type_index(typeid(SystemType)));
        m_systems.erase(system);
    }

    template<typename SystemType>
    bool has_system() const
    {
        return m_systems.find(std::type_index(typeid(SystemType))) != m_systems.end();
    }

    template<typename SystemType>
    SystemType& get_system() const
    {
        auto system = m_systems.find(std::type_index(typeid(SystemType)));
        return *std::static_pointer_cast<SystemType>(system->second);
    }

    void update();

    void add_entity_to_systems(entity e);

private:

    // vector index == component.id
    // pool index == entity.id
    using component_pool_collection = std::vector<pool_base_ptr>;
    // vector index == entity.id
    using signature_collection = std::vector<signature>;
    using system_collection = std::unordered_map<std::type_index, system_ptr>;

    component_pool_collection m_component_pools;
    signature_collection m_entity_component_signatures;
    system_collection m_systems;

    int m_entity_count = 0;

    std::set<entity> m_created_entities;
    std::set<entity> m_destroyed_entities;

};

} // ecs
} // dge

#endif