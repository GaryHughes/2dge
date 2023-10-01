#ifndef dge_ecs_entity_hpp
#define dge_ecs_entity_hpp

namespace dge
{
namespace ecs
{

// class entity
// {
// public:

//     entity(int id) : m_id(id) {}

//     int id() const { return m_id; }

//     bool operator==(const entity& other) const { return m_id == other.m_id; }
//     bool operator<(const entity& other) const { return m_id < other.m_id; }

//     class registry* registry = nullptr;
   
//     template<typename ComponentType, typename ... TArgs> 
//     void add_component(TArgs&& ... args)
//     {
//         registry->add_component<ComponentType>(*this, std::forward<TArgs>(args)...);
//     }

//     template<typename ComponentType> 
//     void remove_component()
//     {
//         registry->remove_component<ComponentType>(*this);
//     }

//     template<typename ComponentType> 
//     bool has_component() const
//     {
//         return registry->has_component<ComponentType>(*this);
//     } 

//     template<typename ComponentType> 
//     ComponentType& get_component() const
//     {
//         return registry->get_component<ComponentType>(*this);
//     } 

// private:

//     int m_id;

// };

} // ecs
} // dge

#endif