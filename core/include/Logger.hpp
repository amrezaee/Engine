#pragma once

#include <Common.hpp>

class Logger
{
public:
	static void Init();

	template<typename... Args>
	static void Trace(const char* fmt, Args&&... args);

	template<typename... Args>
	static void Info(const char* fmt, Args&&... args);

	template<typename... Args>
	static void Warn(const char* fmt, Args&&... args);

	template<typename... Args>
	static void Error(const char* fmt, Args&&... args);

	template<typename... Args>
	static void Fatal(const char* fmt, Args&&... args);

	template<typename T>
	inline static T Argument(T a)
	{
		return a;
	}

	inline static const char* Argument(bool b)
	{
		return b ? "True" : "False";
	}

	inline static const char* Argument(const String& s)
	{
		return s.c_str();
	}

private:
	static char sBuffer[1024];
	static char sFormatBuffer[512];
};  // class Logger

template<typename... Args>
void Logger::Trace(const char* fmt, Args&&... args)
{
	std::snprintf(sFormatBuffer,
	              sizeof(sFormatBuffer),
	              "%s%s%s",
	              "\033[38;2;191;191;191m",
	              fmt,
	              "\033[m\n");
	std::snprintf(sBuffer,
	              sizeof(sBuffer),
	              sFormatBuffer,
	              Argument(std::forward<Args>(args))...);
	std::clog << sBuffer;
}

template<typename... Args>
void Logger::Info(const char* fmt, Args&&... args)
{
	std::snprintf(sFormatBuffer,
	              sizeof(sFormatBuffer),
	              "%s%s%s",
	              "\033[38;2;0;191;0m",
	              fmt,
	              "\033[m\n");
	std::snprintf(sBuffer,
	              sizeof(sBuffer),
	              sFormatBuffer,
	              Argument(std::forward<Args>(args))...);
	std::clog << sBuffer;
}

template<typename... Args>
void Logger::Warn(const char* fmt, Args&&... args)
{
	std::snprintf(sFormatBuffer,
	              sizeof(sFormatBuffer),
	              "%s%s%s",
	              "\033[38;2;191;191;0m",
	              fmt,
	              "\033[m\n");
	std::snprintf(sBuffer,
	              sizeof(sBuffer),
	              sFormatBuffer,
	              Argument(std::forward<Args>(args))...);
	std::clog << sBuffer;
}

template<typename... Args>
void Logger::Error(const char* fmt, Args&&... args)
{
	std::snprintf(sFormatBuffer,
	              sizeof(sFormatBuffer),
	              "%s%s%s",
	              "\033[38;2;127;0;0m",
	              fmt,
	              "\033[m\n");
	std::snprintf(sBuffer,
	              sizeof(sBuffer),
	              sFormatBuffer,
	              Argument(std::forward<Args>(args))...);
	std::cerr << sBuffer;
}

template<typename... Args>
void Logger::Fatal(const char* fmt, Args&&... args)
{
	std::snprintf(sFormatBuffer,
	              sizeof(sFormatBuffer),
	              "%s%s%s",
	              "\033[38;2;255;0;0m",
	              fmt,
	              "\033[m\n");
	std::snprintf(sBuffer,
	              sizeof(sBuffer),
	              sFormatBuffer,
	              Argument(std::forward<Args>(args))...);
	std::cerr << sBuffer;
}

#define TRACE(...) ::Logger::Trace(__VA_ARGS__)
#define INFO(...)  ::Logger::Info(__VA_ARGS__)
#define WARN(...)  ::Logger::Warn(__VA_ARGS__)
#define ERROR(...) ::Logger::Error(__VA_ARGS__)
#define FATAL(...) ::Logger::Fatal(__VA_ARGS__)
