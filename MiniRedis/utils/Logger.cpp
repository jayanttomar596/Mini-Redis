#include "Logger.h"
#include <fstream>

void Logger::log(const std::string &command) {
    std::ofstream file("data.log", std::ios::app);
    file << command << std::endl;
    file.flush();
}