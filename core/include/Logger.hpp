#pragma once

#include <Common.hpp>

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

class Logger
{
public:
	static void Init();

	static std::shared_ptr<spdlog::logger>& GetLogger();
};  // class Logger

#define TRACE(...) ::Logger::GetLogger()->trace(__VA_ARGS__)
#define INFO(...)  ::Logger::GetLogger()->info(__VA_ARGS__)
#define WARN(...)  ::Logger::GetLogger()->warn(__VA_ARGS__)
#define ERROR(...) ::Logger::GetLogger()->error(__VA_ARGS__)
#define FATAL(...) ::Logger::GetLogger()->critical(__VA_ARGS__)