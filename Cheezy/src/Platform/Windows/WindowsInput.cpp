#include "hzpch.h"
#include "WindowsInput.h"

#include "Cheezy/Core/Window.h"
#include "Cheezy/Core/Application.h"

#include <GLFW/glfw3.h>

static inline GLFWwindow* GetWindow()
{
	return static_cast<GLFWwindow*>(Cheezy::Application::Get()->GetWindow()->GetNativeWindow());
}

namespace Cheezy
{
	bool WindowsInput::IsKeyPressedImpl(KeyCode keycode)
	{
		return glfwGetKey(GetWindow(), static_cast<int32_t>(keycode)) == GLFW_PRESS;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(MouseCode button)
	{
		return glfwGetMouseButton(GetWindow(), static_cast<int32_t>(button)) == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePosImpl()
	{
		double mouseX, mouseY;
		glfwGetCursorPos(GetWindow(), &mouseX, &mouseY);
		return std::pair<float, float>{ (float)mouseX, (float)mouseY };
	}
}
