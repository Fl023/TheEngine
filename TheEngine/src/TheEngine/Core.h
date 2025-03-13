#pragma once

#ifdef TE_PLATFORM_WINDOWS
	#ifdef TE_BUILD_DLL
		#define THEENGINE_API __declspec(dllexport)
	#else
		#define THEENGINE_API __declspec(dllimport)
	#endif
#else
	#error TheEngine only supports Windows!
#endif