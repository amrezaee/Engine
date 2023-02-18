#include <Logger.hpp>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

static std::shared_ptr<spdlog::logger> logger;

void Logger::Init()
{
	Vector<spdlog::sink_ptr> sinks;
	sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	sinks.emplace_back(
	        std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt", true));

	sinks[0]->set_pattern("[%t] [%T] [%n] %^[%=8l] | %v%$");
	sinks[1]->set_pattern("[%t] [%T] [%n] [%=8l] | %v");

	logger = MakeSptr<spdlog::logger>("Engine", begin(sinks), end(sinks));
	spdlog::register_logger(logger);
	logger->set_level(spdlog::level::trace);
	logger->flush_on(spdlog::level::trace);
}

Sptr<spdlog::logger>& Logger::GetLogger()
{
	return logger;
}
