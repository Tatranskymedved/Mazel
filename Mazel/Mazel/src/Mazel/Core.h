#pragma once

#ifdef MZ_PLATFORM_WINDOWS
	#ifdef MZ_BUILD_DLL
		#define MAZEL_API __declspec(dllexport)
	#else
		#define MAZEL_API __declspec(dllimport)
	#endif // MZ_BUILD_DLL
#else
	#error Mazel only supports Windows!
#endif //MZ_PLATFORM_WINDOWS

#ifdef MZ_ENABLE_ASSERTS
	#define MZ_ASSERT(x, ...) { if(!(x)) { MZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MZ_CORE_ASSERT(x, ...) { if(!(x)) { MZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MZ_ASSERT(x, ...)
	#define MZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)