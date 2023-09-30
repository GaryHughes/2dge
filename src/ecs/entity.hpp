#ifndef dge_ecs_entity_hpp
#define dge_ecs_entity_hpp

namespace dge
{
namespace ecs
{

class entity
{
public:

    entity(int id) : m_id(id) {}

    int id() const { return m_id; }

    bool operator==(const entity& other) const { return m_id == other.m_id; }

private:

    int m_id;

};

} // ecs
} // dge

#endif