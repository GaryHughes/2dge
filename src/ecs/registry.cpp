#include "registry.hpp"
#include "../logger.hpp"

namespace dge
{
namespace ecs
{

entity registry::create_entity()
{
    int id = m_entity_count++;
    if (id >= m_entity_component_signatures.size()) {
        m_entity_component_signatures.resize(id + 1);
    }
    auto e = entity(id);
    // e.registry = this;
    m_created_entities.insert(e);
    logger::info("entity created with id = " + std::to_string(id));
    return e;
}

void registry::destroy_entity(entity& e)
{

}

void registry::update()
{
    for (const auto& entity : m_created_entities) {
        add_entity_to_systems(entity);
    }
    m_created_entities.clear();
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

} // ecs
} // dge
