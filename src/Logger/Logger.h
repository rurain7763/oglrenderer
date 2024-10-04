#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    static void Info(const std::string& msg);
    static void Err(const std::string& msg);
	static void Warn(const std::string& msg);
};

#define LOG_INFO(fmt, ...) \
	{ \
		char buffer[256]; \
		snprintf(buffer, sizeof(buffer), "[%s: %d][%s] " fmt, \
		    __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
		Logger::Info(buffer); \
	}

#define LOG_ERR(fmt, ...) \
	{ \
		char buffer[256]; \
		snprintf(buffer, sizeof(buffer), "[%s: %d][%s] " fmt, \
		    __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
		Logger::Err(buffer); \
	}

#define LOG_WARN(fmt, ...) \
	{ \
		char buffer[256]; \
		snprintf(buffer, sizeof(buffer), "[%s: %d][%s] " fmt, \
		    __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
		Logger::Warn(buffer); \
	}

#endif