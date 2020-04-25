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

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		/*CZ_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(CZ_KEY_A))
			m_Position.x -= m_MoveSpeed * ts;
		if (Input::IsKeyPressed(CZ_KEY_D))
			m_Position.x += m_MoveSpeed * ts;

		if (Input::IsKeyPressed(CZ_KEY_W))
			m_Position.y += m_MoveSpeed * ts;
		if (Input::IsKeyPressed(CZ_KEY_S))
			m_Position.y -= m_MoveSpeed * ts;

		if (m_CanRotate)
		{
			if (Input::IsKeyPressed(CZ_KEY_Q))
				m_Rotation += m_RotateSpeed * ts;
			if (Input::IsKeyPressed(CZ_KEY_E))
				m_Rotation -= m_RotateSpeed * ts;

			if (m_Rotation > 180.0f)
				m_Rotation -= 360.0f;
			else if (m_Rotation <= -180.0f)
				m_Rotation += 360.0f;

			m_Camera.SetRotation(m_Rotation);
		}

		m_Camera.SetPosition(m_Position);*/
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