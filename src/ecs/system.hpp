#ifndef dge_system_hpp
#define dge_system_hpp

#include <vector>
#include "entity.hpp"
#include "component.hpp"

namespace dge
{
namespace ecs
{

class system
{
public:

    using entity_collection = std::vector<entity>;

    void add(const entity& e);
    void remove(const entity& e);

    signature component_signature() const { return m_component_signature; }
    const entity_collection& entities() const { return m_entities; }

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

} // ecs
} // dge

#endif