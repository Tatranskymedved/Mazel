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