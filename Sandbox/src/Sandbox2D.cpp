#include "Sandbox2D.h"

#include "Hazel.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox 2D"),
	m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Hazel::RenderCommand::SetClearColor(glm::vec4{ 0.1, 0.1, 0.1, 1 });
		Hazel::RenderCommand::Clear();
	}

	{
		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

		//Non-Rotated Flat Color
		Hazel::Renderer2D::DrawQuad({ -1.0f, 1.0f }, { 1.0f, 1.0f }, m_SquareColor);

		//Not-Rotated Texture
		Hazel::Renderer2D::DrawQuad({ -1.0f, -1.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture, 1.0f, m_SquareColor);

		//Rotated Flat Color
		Hazel::Renderer2D::DrawRotatedQuad({ 1.0f, 1.0f }, { 1.0f, 1.0f }, m_SquareRotation, m_SquareColor);

		//Rotated Texture
		Hazel::Renderer2D::DrawRotatedQuad({ 1.0f, -1.0f }, { 1.0f, 1.0f }, m_SquareRotation, m_ChernoLogoTexture, 1.0f, m_SquareColor);

		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::DragFloat("Square Rotation", &m_SquareRotation, 0.2f, -180.0f, 180.0f);
	ImGui::End();
}
