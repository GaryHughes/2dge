#ifndef dge_ecs_pool_hpp
#define dge_ecs_pool_hpp

#include <vector>
#include <unordered_map>
#include "../logger.hpp"

namespace dge
{
namespace ecs
{

class pool_base 
{
public:

    pool_base() = default;
    virtual ~pool_base() = default;

    virtual void remove_entity_from_pool(int entity_id) = 0;

};

using pool_base_ptr = std::shared_ptr<pool_base>;

template<typename T>
class pool : public pool_base
{
public:

    pool(size_t capacity = 100) 
    {
        m_size = 0;
        m_data.resize(capacity);
    }

    void set(int entity_id, T object)
    {
        if (m_entity_id_to_index.find(entity_id) != m_entity_id_to_index.end()) {
            int index = m_entity_id_to_index[entity_id];
            m_data[index] = object;
        }
        else {
            size_t index = m_size;
            m_entity_id_to_index.emplace(entity_id, index);
            m_index_to_entity_id.emplace(index, entity_id);
            if (index > m_data.size() - 1) {
                m_data.resize(m_data.size() * 2);
            }
            m_data[index] = object;
            m_size++;
        }
    }

    virtual void remove_entity_from_pool(int entity_id) override
    {
        if (m_entity_id_to_index.find(entity_id) != m_entity_id_to_index.end()) {
            remove(entity_id);
        }
    }

    void remove(int entity_id)
    {
        size_t index_of_removed = m_entity_id_to_index[entity_id];
        size_t index_of_last = m_size - 1;
        m_data[index_of_removed] = m_data[index_of_last];

        int entity_id_of_last_element = m_index_to_entity_id[index_of_last];
        m_entity_id_to_index[entity_id_of_last_element] = index_of_removed;
        m_index_to_entity_id[index_of_removed] = entity_id_of_last_element;

        m_entity_id_to_index.erase(entity_id);
        m_index_to_entity_id.erase(index_of_last);

        m_size--;
    }

    bool empty() const { return m_size == 0; }
    size_t size() const { return m_size; }
    
    void clear() 
    { 
        m_data.clear(); 
        m_entity_id_to_index.clear();
        m_index_to_entity_id.clear();
        m_size = 0;
    }

    T& get(int entity_id) 
    {
        size_t index = m_entity_id_to_index[entity_id]; 
        return m_data[index]; 
    }

    T& operator[](size_t entity_id) { return get(entity_id); }

private:

    std::vector<T> m_data;
    size_t m_size;
    std::unordered_map<int, size_t> m_entity_id_to_index;
    std::unordered_map<size_t, int> m_index_to_entity_id;

};

} // ecs
} // dge

#endif