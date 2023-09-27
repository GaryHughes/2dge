#include "logger.hpp"
#include <iostream>
#include <chrono>
#include <sstream>

namespace dge
{

std::vector<logger::log_entry> logger::s_messages;

const char* color_reset = "\033[0m";
const char* color_red   = "\033[31m";
const char* color_green = "\033[32m";

std::string time_to_string(const std::chrono::system_clock::time_point &t)
{
    std::time_t time = std::chrono::system_clock::to_time_t(t);
    std::string time_str = std::ctime(&time);
    time_str.resize(time_str.size() - 1);
    return time_str;
}

void logger::info(const std::string& message)
{
    std::ostringstream buffer;
    buffer << time_to_string(std::chrono::system_clock::now()) << " [INF] " << message;
    s_messages.emplace_back(log_entry { log_type::info, buffer.str() });
    std::cout << color_green << buffer.str() << '\n' << color_reset;
}

void logger::error(const std::string& message)
{
    std::ostringstream buffer;
    buffer << time_to_string(std::chrono::system_clock::now()) << " [ERR] " << message;
    s_messages.emplace_back(log_entry { log_type::error, buffer.str() });
    std::cerr << color_red << buffer.str() << '\n' << color_reset;
}

}