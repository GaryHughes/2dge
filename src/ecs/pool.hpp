#ifndef dge_ecs_pool_hpp
#define dge_ecs_pool_hpp

#include <vector>

namespace dge
{
namespace ecs
{

class pool_base 
{
public:

    pool_base() = default;
    virtual ~pool_base() {}

};

using pool_base_ptr = std::shared_ptr<pool_base>;

template<typename T>
class pool : public pool_base
{
public:

    pool(size_t size = 100) { resize(size); }

    bool empty() const { return m_data.empty(); }
    size_t size() const { return m_data.size(); }
    void resize(size_t n) { m_data.resize(n); }
    void clear() { m_data.clear(); }
    void push_back(const T& object) { m_data.push_back(object); }
    void set(size_t index, const T& object) { m_data[index] = object; }
    T& get(size_t index) { return m_data[index]; }
    T& operator[](size_t index) { return m_data[index]; }

private:

    std::vector<T> m_data;

};

} // ecs
} // dge

#endif