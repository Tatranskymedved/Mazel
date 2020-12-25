#pragma once

#include "memory"

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Mazel
{

	class MAZEL_API Log
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


//Core log macros
#define MZ_CORE_TRACE(...)      ::Mazel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MZ_CORE_INFO(...)       ::Mazel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MZ_CORE_WARN(...)       ::Mazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MZ_CORE_ERROR(...)      ::Mazel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MZ_CORE_FATAl(...)      ::Mazel::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define MZ_TRACE(...)           ::Mazel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MZ_INFO(...)            ::Mazel::Log::GetClientLogger()->info(__VA_ARGS__)
#define MZ_WARN(...)            ::Mazel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MZ_ERROR(...)           ::Mazel::Log::GetClientLogger()->error(__VA_ARGS__)
#define MZ_FATAl(...)           ::Mazel::Log::GetClientLogger()->fatal(__VA_ARGS__)