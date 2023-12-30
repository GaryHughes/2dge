#ifndef dge_ecs_registry_hpp
#define dge_ecs_registry_hpp

#include <vector>
#include <deque>
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

class entity
{
public:

    entity(int id) : m_id(id) {}

    void destroy();

    int id() const { return m_id; }

    bool operator==(const entity& other) const { return m_id == other.m_id; }
    bool operator<(const entity& other) const { return m_id < other.m_id; }

    class registry* registry = nullptr;
   
    template<typename ComponentType, typename ... TArgs> 
    void add_component(TArgs&& ... args);

    template<typename ComponentType> 
    void remove_component();

    template<typename ComponentType> 
    bool has_component() const;

    template<typename ComponentType> 
    ComponentType& get_component();

    void tag(const std::string& tag);
    bool has_tag(const std::string& tag);
    void group(const std::string& group);
    bool has_group(const std::string& group);

private:

    int m_id;

};

class system
{
public:

    using entity_collection = std::vector<entity>;

    void add(const entity& e)
    {
        m_entities.push_back(e);
    }

    void remove(const entity& e)
    {
        m_entities.erase(
            std::remove_if(
                m_entities.begin(), 
                m_entities.end(), 
                [&] (entity& other) {
                    return e == other;
                }
            ),
            m_entities.end()
        );
    }

    signature component_signature() const { return m_component_signature; }
    entity_collection& entities() { return m_entities; }

protected:

    template <typename ComponentType> void require_component()
    {
        const auto component_id = component<ComponentType>::id();
        m_component_signature.set(component_id);
    }

private:

    signature m_component_signature;

    entity_collection m_entities;

};

using system_ptr = std::shared_ptr<system>;

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
   
        std::shared_ptr<pool<ComponentType>> p = std::static_pointer_cast<pool<ComponentType>>(m_component_pools[component_id]);
        p.remove(entity_id);
   
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
        return (*component_pool)[entity_id];
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
    void remove_entity_from_systems(entity e);

    void tag_entity(const ecs::entity& entity, const std::string& tag);
    bool entity_has_tag(const ecs::entity& entity, const std::string& tag) const;
    ecs::entity get_entity_by_tag(const std::string& tag) const;
    void remove_entity_by_tag(const ecs::entity& entity);

    void group_entity(const ecs::entity entity, const std::string& group);
    bool entity_belongs_to_group(const ecs::entity& entity, const std::string& group) const;
    std::vector<ecs::entity> get_entities_by_group(const std::string& group) const;
    void remove_entity_group(const ecs::entity entity);

private:

    // vector index == component.id
    // pool index == entity.id
    using component_pool_collection = std::vector<pool_base_ptr>;
    // vector index == entity.id
    using signature_collection = std::vector<signature>;
    using system_collection = std::unordered_map<std::type_index, system_ptr>;
    using id_collection = std::deque<int>;

    component_pool_collection m_component_pools;
    signature_collection m_entity_component_signatures;
    system_collection m_systems;

    int m_entity_count = 0;

    std::set<entity> m_created_entities;
    std::set<entity> m_destroyed_entities;
    id_collection m_free_ids;

    std::unordered_map<std::string, ecs::entity> m_entity_per_tag;
    std::unordered_map<int, std::string> m_tag_per_entity;

    std::unordered_map<std::string, std::set<ecs::entity>> m_entities_per_group;
    std::unordered_map<int, std::string> m_group_per_entity;

};

template<typename ComponentType, typename ... TArgs> 
void entity::add_component(TArgs&& ... args)
{
    registry->add_component<ComponentType>(*this, std::forward<TArgs>(args)...);
}

template<typename ComponentType> 
void entity::remove_component()
{
    registry->remove_component<ComponentType>(*this);
}

template<typename ComponentType> 
bool entity::has_component() const
{
    return registry->has_component<ComponentType>(*this);
} 

template<typename ComponentType> 
ComponentType& entity::get_component()
{
    return registry->get_component<ComponentType>(*this);
} 

} // ecs
} // dge

#endif