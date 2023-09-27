#ifndef dge_logger_hpp
#define dge_logger_hpp

#include <string>
#include <vector>

namespace dge
{

class logger
{
public:

    static void info(const std::string& message);
    static void error(const std::string& message);

private:

    enum class log_type
    {
        info,
        warning,
        error
    };

    struct log_entry
    {
        log_type type;
        std::string message;
    };

    static std::vector<log_entry> s_messages;

};

}

#endif