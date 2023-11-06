#include "registry.hpp"
#include "../logger.hpp"

namespace dge
{
namespace ecs
{

void entity::destroy()
{
    registry->destroy_entity(*this);
}

entity registry::create_entity()
{
    int id;

    if (m_free_ids.empty()) {
        id = m_entity_count++;
        if (id >= m_entity_component_signatures.size()) {
            m_entity_component_signatures.resize(id + 1);
        }
    }
    else {
        id = m_free_ids.front();
        m_free_ids.pop_front();
    }

    auto e = entity(id);
    e.registry = this;
    m_created_entities.insert(e);
    logger::info("entity created with id = " + std::to_string(id));
    return e;
}

void registry::destroy_entity(entity& e)
{
    m_destroyed_entities.insert(e);
}

void registry::update()
{
    for (const auto& entity : m_created_entities) {
        add_entity_to_systems(entity);
    }
    
    m_created_entities.clear();
    
    for (const auto& entity : m_destroyed_entities) {
        remove_entity_from_systems(entity);
        m_entity_component_signatures[entity.id()].reset();
        m_free_ids.push_back(entity.id());
    }

    m_destroyed_entities.clear();
}

void registry::add_entity_to_systems(entity e)
{
    const auto entity_id = e.id();
    const auto& entity_component_signature = m_entity_component_signatures[entity_id];
    for (auto& system : m_systems) {
        const auto& system_component_signature = system.second->component_signature();
        bool is_interested = (entity_component_signature & system_component_signature) == system_component_signature;
        if (is_interested) {
            system.second->add(e);
        }
    }
}

void registry::remove_entity_from_systems(entity e)
{
    for (auto& system : m_systems) {
       system.second->remove(e);
    }    
}

} // ecs
} // dge
