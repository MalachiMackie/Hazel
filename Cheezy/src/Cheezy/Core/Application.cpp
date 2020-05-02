#include "hzpch.h"
#include "Application.h"

#include "Cheezy/Renderer/Renderer.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <chrono>

namespace Cheezy
{
	Application* Application::s_Instance = nullptr;

	const glm::vec2 Application::m_Gravity = glm::vec2(0.0f, -9.81f);
	float Application::m_FixedFrameTime = 1.0f/50.0f;

	Application::Application()
	{
		CZ_PROFILE_FUNCTION();

		CZ_CORE_ASSERT(!s_Instance, "Application already exists!")
			s_Instance = this;

		m_Window = Window::Create(WindowProps{ "Cheezy Engine", 1280, 720, /*VSync*/false });
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
		using clock = std::chrono::high_resolution_clock;

		const std::chrono::milliseconds fixedFrameTime((int)(m_FixedFrameTime * 1000.0f));

		std::chrono::nanoseconds lag(0);
		auto timeStart = clock::now();

		CZ_PROFILE_FUNCTION();

		while (m_Running)
		{
			CZ_PROFILE_SCOPE("RunLoop");

			auto currentTime = clock::now();
			auto frameTime = currentTime - timeStart;

			static int fixedUpdates = 0;
			static int updates = 0;
			static float frameTimeSum = 0;
			
			auto milliseconds = std::chrono::duration_cast<std::chrono::microseconds>(frameTime).count() / 1000.0f;
			Timestep timestep = milliseconds / 1000.0f;
			if (std::chrono::time_point_cast<std::chrono::seconds>(timeStart) != std::chrono::time_point_cast<std::chrono::seconds>(currentTime))
			{
				CZ_CORE_TRACE("FixedUpdates: {0}, Updates: {1}, Avg Frame Time: {2}", fixedUpdates, updates, (frameTimeSum / updates));
				fixedUpdates = 0;
				updates = 0;
				frameTimeSum = 0;
			}
			timeStart = currentTime;

			lag += frameTime;

			frameTimeSum += timestep;

			if (!m_Minimized)
			{
				CZ_PROFILE_SCOPE("LayerStack OnUpdate");

				++updates;
				for (Ref<Layer> layer : m_LayerStack)
					layer->OnUpdate(timestep);
				
				while (lag >= fixedFrameTime)
				{
					++fixedUpdates;
					lag -= fixedFrameTime;
					for (Ref<Layer> layer : m_LayerStack)
						layer->OnFixedUpdate();
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