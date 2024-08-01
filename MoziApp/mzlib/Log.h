#pragma once

//#include <memory>
#include "spdlog/spdlog.h"

namespace MoLog
{
	class Log
	{
	public:
		static void LogInit();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}



// Core log macros
#define LOG_TRACE(...)    ::MoLog::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)     ::MoLog::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)     ::MoLog::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)    ::MoLog::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::MoLog::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LOG_CLIENT_TRACE(...)         ::MoLog::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_CLIENT_INFO(...)          ::MoLog::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_CLIENT_WARN(...)          ::MoLog::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_CLIENT_ERROR(...)         ::MoLog::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CLIENT_CRITICAL(...)      ::MoLog::Log::GetClientLogger()->critical(__VA_ARGS__)

