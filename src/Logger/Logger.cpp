#include "Logger.h"

#include "../../vendor/spdlog/spdlog.h"

void Logger::Info(const std::string& msg) {
    spdlog::info(msg);
}

void Logger::Err(const std::string& msg) {
    spdlog::error(msg);
}

void Logger::Warn(const std::string& msg) {
    spdlog::warn(msg);
}