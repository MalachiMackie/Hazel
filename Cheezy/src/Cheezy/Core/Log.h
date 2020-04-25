#pragma once

#include "Cheezy/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Cheezy
{
	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

// Coro log marcos
#define CZ_CORE_TRACE(...)		::Cheezy::Log::GetCoreLogger()  ->trace(__VA_ARGS__)
#define CZ_CORE_INFO(...)		::Cheezy::Log::GetCoreLogger()  ->info(__VA_ARGS__)
#define CZ_CORE_WARN(...)		::Cheezy::Log::GetCoreLogger()  ->warn(__VA_ARGS__)
#define CZ_CORE_ERROR(...)		::Cheezy::Log::GetCoreLogger()  ->error(__VA_ARGS__)
#define CZ_CORE_CRITICAL(...)	::Cheezy::Log::GetCoreLogger()  ->critical(__VA_ARGS__)

#define CZ_TRACE(...)			::Cheezy::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CZ_INFO(...)			::Cheezy::Log::GetClientLogger()->info(__VA_ARGS__)
#define CZ_WARN(...)			::Cheezy::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CZ_ERROR(...)			::Cheezy::Log::GetClientLogger()->error(__VA_ARGS__)
#define CZ_CRITICAL(...)		::Cheezy::Log::GetClientLogger()->critical(__VA_ARGS__)

