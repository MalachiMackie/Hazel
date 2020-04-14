#pragma once

#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Hazel
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool canRotate = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_CanRotate;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation;
		float m_MoveSpeed = 2.0f;
		float m_RotateSpeed = 90.0f;
	};
}