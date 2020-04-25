#pragma once

#include "Cheezy/Core/Core.h"
#include "Cheezy/Core/Window.h"
#include "Cheezy/Core/LayerStack.h"
#include "Cheezy/Events/Event.h"
#include "Cheezy/Events/ApplicationEvent.h"

#include "Cheezy/Core/Timestep.h"

#include "Cheezy/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Cheezy
{
	class Application
	{
	public:

		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Ref<Layer> layer);
		void PushOverlay(Ref<Layer> overlay);

		inline static Application* Get() { return s_Instance; }
		inline const Scope<Window>& GetWindow() const { return m_Window; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<Window> m_Window;
		Ref<ImGuiLayer> m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

