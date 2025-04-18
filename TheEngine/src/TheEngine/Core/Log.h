#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace TheEngine {
	class THEENGINE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define TE_CORE_TRACE(...)    ::TheEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TE_CORE_INFO(...)     ::TheEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TE_CORE_WARN(...)     ::TheEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TE_CORE_ERROR(...)    ::TheEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TE_CORE_CRITICAL(...) ::TheEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define TE_TRACE(...)         ::TheEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TE_INFO(...)          ::TheEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define TE_WARN(...)          ::TheEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TE_ERROR(...)         ::TheEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define TE_CRITICAL(...)      ::TheEngine::Log::GetClientLogger()->critical(__VA_ARGS__)




