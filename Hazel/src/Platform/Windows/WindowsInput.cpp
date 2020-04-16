#include "hzpch.h"
#include "WindowsInput.h"

#include "Hazel/Core/Window.h"
#include "Hazel/Core/Application.h"

#include <GLFW/glfw3.h>

static inline GLFWwindow* GetWindow()
{
	return static_cast<GLFWwindow*>(Hazel::Application::Get().GetWindow().GetNativeWindow());
}

namespace Hazel
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
