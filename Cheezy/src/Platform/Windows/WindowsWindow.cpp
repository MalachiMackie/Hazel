#include "hzpch.h"
#include "WindowsWindow.h"

#include "Cheezy/Events/ApplicationEvent.h"
#include "Cheezy/Events/MouseEvent.h"
#include "Cheezy/Events/KeyEvent.h"
#include "Cheezy/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Cheezy
{
	static uint32_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		CZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		CZ_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		CZ_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		CZ_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.VSync = props.VSync;

		CZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			CZ_PROFILE_SCOPE("glfwInit");

		#if defined(CZ_DEBUG)
			if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		#endif

			int success = glfwInit();
			CZ_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			CZ_PROFILE_SCOPE("glfwCreateWindow");
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		}
		++s_GLFWWindowCount;

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(m_Data.VSync);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event{};
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			Event* event{ nullptr };

			switch (action)
			{
				case GLFW_PRESS:
				{
					event = (Event*)&KeyPressedEvent{ static_cast<KeyCode>(key), 0 };
					break;
				}
				case GLFW_RELEASE:
				{
					event = (Event*)&KeyReleasedEvent{ static_cast<KeyCode>(key) };
					break;
				}
				case GLFW_REPEAT:
				{
					event = (Event*)&KeyPressedEvent{ static_cast<KeyCode>(key), 1};
					break;
				}
			}

			CZ_CORE_ASSERT(event, "Key event is of the wrong type: " + std::to_string(action));
			if (event)
			{
				data.EventCallback(*event);
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int c)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event{ static_cast<KeyCode>(c) };
			data.EventCallback(event);
		});



		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			Event* event{ nullptr };
			switch (action)
			{
				case GLFW_PRESS:
				{
					event = (Event*)&MouseButtonPressedEvent(static_cast<MouseCode>(button));
					break;
				}
				case GLFW_RELEASE:
				{
					event = (Event*)&MouseButtonReleasedEvent(static_cast<MouseCode>(button));
					break;
				}
			}

			CZ_CORE_ASSERT(event, "Mouse event of of the wrong type: " + std::to_string(action));
			if (event)
			{
				data.EventCallback(*event);
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event{ (float)xOffset, (float)yOffset };
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event{ (float)xPos, (float)yPos };
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		CZ_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);

		--s_GLFWWindowCount;
		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		CZ_PROFILE_FUNCTION();

		{
			CZ_PROFILE_SCOPE("glfwPollEvents");
			glfwPollEvents();
		}
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		CZ_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}