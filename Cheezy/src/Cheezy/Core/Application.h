#pragma once

#include "Cheezy/Core/Core.h"
#include "Cheezy/Core/Window.h"
#include "Cheezy/Core/LayerStack.h"
#include "Cheezy/Core/Scene2D.h"
#include "Cheezy/Events/Event.h"
#include "Cheezy/Events/ApplicationEvent.h"

#include "Cheezy/ImGui/ImGuiLayer.h"

#include <glm/glm.hpp>

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

		static Application* Get() { return s_Instance; }
		const Scope<Window>& GetWindow() const { return m_Window; }
		const Ref<Scene2D> GetScene() const { return m_Scene; }

		static const glm::vec2& GetGravity() { return m_Gravity; }
		static float GetFixedFrameTime() { return m_FixedFrameTime; }

		//Temporary
		void SetScene(const Ref<Scene2D>& scene) { m_Scene = scene; }

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
		Ref<Scene2D> m_Scene;

		static float m_FixedFrameTime;
		static const glm::vec2 m_Gravity;
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

