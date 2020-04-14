#include "hzpch.h"
#include "WindowsInput.h"

#include "Hazel/Window.h"
#include "Hazel/Application.h"

#include <GLFW/glfw3.h>

static inline GLFWwindow* GetWindow()
{
	return static_cast<GLFWwindow*>(Hazel::Application::Get().GetWindow().GetNativeWindow());
}

namespace Hazel
{
	Input* Input::s_Instance = new WindowsInput();

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
