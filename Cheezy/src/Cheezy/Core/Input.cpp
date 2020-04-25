#include "hzpch.h"
#include "Cheezy/Core/Input.h"

#ifdef CZ_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsInput.h"
#endif

namespace Cheezy {

	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
	#ifdef CZ_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
	#else
		CZ_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}
}
