#pragma once

#include <memory>

#ifdef _WIN32
	#ifdef _WIN64
		#ifndef CZ_PLATFORM_WINDOWS
			#define CZ_PLATFORM_WINDOWS
		#endif
	#else
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on a MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported"
	#elif TARGET_OS_IPHONE == 1
		#define CZ_PLATFORM_IOS
		#error "IOS is not supported."
	#elif TARGET_OS_MAC == 1
		#define CZ_PLATFORM_MACOS
		#error "MacOS is not supported"
	#else
		#error "Unknown Apple platform"
	#endif
#elif defined(__ANDROID__)
	#define CZ_PLATFORM_ANDROID
	#error "Android is not supported"
#elif defined(__linux__)
	#define CZ_PLATFORM_LINUX
	#error "Linux is not supported"
#else
	#error "Unknown platform"
#endif

#ifdef CZ_DEBUG
	#define CZ_ENABLE_ASSERTS
#endif

#ifdef CZ_ENABLE_ASSERTS
	#define CZ_ASSERT(x, ...) { if (!(x)) { CZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CZ_CORE_ASSERT(x, ...) { if (!(x)) { CZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CZ_ASSERT(x, ...)
	#define CZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define CZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Cheezy
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}