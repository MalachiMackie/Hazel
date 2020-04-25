#include "hzpch.h"
#include "Application.h"

#include "Cheezy/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Cheezy
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		CZ_PROFILE_FUNCTION();

		CZ_CORE_ASSERT(!s_Instance, "Application already exists!")
			s_Instance = this;

		m_Window = Window::Create(WindowProps{ "Cheezy Engine", 1280, 720, true });
		m_Window->SetEventCallback(CZ_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = CreateRef<ImGuiLayer>();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		CZ_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Ref<Layer> layer)
	{
		CZ_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Ref<Layer> overlay)
	{
		CZ_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		CZ_PROFILE_FUNCTION();

		EventDispatcher dispatcher{ e };
		dispatcher.Dispatch<WindowCloseEvent>(CZ_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(CZ_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto layer = m_LayerStack.rbegin(); layer != m_LayerStack.rend(); ++layer)
		{
			(*layer)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		CZ_PROFILE_FUNCTION();

		while (m_Running)
		{
			CZ_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime(); // Platform::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					CZ_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Ref<Layer> layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}
			}

			// ImGui Layer
			m_ImGuiLayer->Begin();
			{
				CZ_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Ref<Layer> layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		CZ_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}