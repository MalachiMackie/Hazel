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
	Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		return glfwGetKey(GetWindow(), keycode) == GLFW_PRESS;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		return glfwGetMouseButton(GetWindow(), button) == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePosImpl()
	{
		double mouseX, mouseY;
		glfwGetCursorPos(GetWindow(), &mouseX, &mouseY);
		return std::pair<float, float>{ (float)mouseX, (float)mouseY };
	}
}
