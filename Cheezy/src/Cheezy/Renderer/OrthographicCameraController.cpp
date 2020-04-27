#include "hzpch.h"
#include "OrthographicCameraController.h"

#include "Cheezy/Core/Input.h"
#include "Cheezy/Core/KeyCodes.h"

namespace Cheezy
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool canRotate)
		: m_AspectRatio(aspectRatio),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_CanRotate(canRotate)
	{
	}

	void OrthographicCameraController::OnFixedUpdate()
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		CZ_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(CZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(CZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		CZ_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.1f;
		m_ZoomLevel = std::max(0.25f, m_ZoomLevel);
		m_MoveSpeed = m_ZoomLevel;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		CZ_PROFILE_FUNCTION();

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}