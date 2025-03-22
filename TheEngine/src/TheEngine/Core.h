#pragma once

#ifdef TE_PLATFORM_WINDOWS
#if TE_DYNAMIC_LINK
	#ifdef TE_BUILD_DLL
		#define THEENGINE_API __declspec(dllexport)
	#else
		#define THEENGINE_API __declspec(dllimport)
	#endif
#else
	#define THEENGINE_API
#endif
#else
	#error TheEngine only supports Windows!
#endif

#ifdef TE_DEBUG
	#define TE_ENABLE_ASSERTS
#endif

#ifdef TE_ENABLE_ASSERTS
	#define TE_ASSERT(x, ...) { if(!(x)) { TE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define TE_CORE_ASSERT(x, ...) { if(!(x)) { TE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define TE_ASSERT(x, ...)
	#define TE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define TE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)