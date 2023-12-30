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

void entity::tag(const std::string& tag) 
{
    registry->tag_entity(*this, tag); 
}

bool entity::has_tag(const std::string& tag) 
{ 
    return registry->entity_has_tag(*this, tag); 
}

void entity::group(const std::string& group) 
{ 
    registry->group_entity(*this, group); 
}

bool entity::has_group(const std::string& group) 
{ 
    return registry->entity_belongs_to_group(*this, group); 
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

        for (auto pool : m_component_pools) {
            if (pool) {
                pool->remove_entity_from_pool(entity.id());
            }
        }

        m_free_ids.push_back(entity.id());
        remove_entity_by_tag(entity);
        remove_entity_group(entity);
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

void registry::tag_entity(const ecs::entity& entity, const std::string& tag)
{
    m_entity_per_tag.emplace(tag, entity);
    m_tag_per_entity.emplace(entity.id(), tag);
}

bool registry::entity_has_tag(const ecs::entity& entity, const std::string& tag) const
{
    if (m_tag_per_entity.find(entity.id()) == m_tag_per_entity.end()) {
        return false;
    }
    return m_entity_per_tag.find(tag)->second == entity;
}

ecs::entity registry::get_entity_by_tag(const std::string& tag) const
{
    return m_entity_per_tag.at(tag);
}

void registry::remove_entity_by_tag(const ecs::entity& entity)
{
    auto tagged_entity = m_tag_per_entity.find(entity.id());
    if (tagged_entity != m_tag_per_entity.end()) {
        auto tag = tagged_entity->second;
        m_entity_per_tag.erase(tag);
        m_tag_per_entity.erase(tagged_entity);
    }
}

void registry::group_entity(const ecs::entity entity, const std::string& group)
{
    m_entities_per_group.emplace(group, std::set<ecs::entity>());
    m_entities_per_group[group].emplace(entity);
    m_group_per_entity.emplace(entity.id(), group);

}

bool registry::entity_belongs_to_group(const ecs::entity& entity, const std::string& group) const
{
    if (m_entities_per_group.find(group) == m_entities_per_group.end()) {
        return false;
    }
    auto group_entities = m_entities_per_group.at(group);
    return group_entities.find(entity.id()) != group_entities.end();
}

std::vector<ecs::entity> registry::get_entities_by_group(const std::string& group) const
{
    if (m_entities_per_group.find(group) == m_entities_per_group.end()) {
        return std::vector<ecs::entity>();
    }
    auto& set_of_entities = m_entities_per_group.at(group);
    return std::vector<ecs::entity>(set_of_entities.begin(), set_of_entities.end());
}

void registry::remove_entity_group(const ecs::entity entity)
{
    auto grouped_entity = m_group_per_entity.find(entity.id());
    if (grouped_entity != m_group_per_entity.end()) {
        auto group = m_entities_per_group.find(grouped_entity->second);
        if (group != m_entities_per_group.end()) {
            auto entity_in_group = group->second.find(entity);
            if (entity_in_group != group->second.end()) {
                group->second.erase(entity_in_group);
            }
        }
        m_group_per_entity.erase(grouped_entity);
    }
}

} // ecs
} // dge
