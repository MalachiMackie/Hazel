#include "hzpch.h"
#include "Cheezy/Core/Window.h"

#ifdef CZ_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Cheezy
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
	#ifdef CZ_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
	#else
		CZ_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}

}