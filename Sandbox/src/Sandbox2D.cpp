#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox 2D"),
	m_Scene({ 1280.0f / 720.0f, true })
{
}

void Sandbox2D::OnAttach()
{
	CZ_PROFILE_FUNCTION();

	m_CheckerboardTexture = Cheezy::Texture2D::Create("assets/textures/Checkerboard.png");
	m_ChernoLogoTexture = Cheezy::Texture2D::Create("assets/textures/ChernoLogo.png");

	auto mObject = Cheezy::CreateRef<Cheezy::CheezyObject>();
	mObject->AddComponent(Cheezy::CreateRef<Cheezy::Transform2DComponent>(glm::vec3(2.0f, 0.0f, 0.0f)));
	mObject->AddComponent(Cheezy::CreateRef<Cheezy::ScriptComponent>("src/scripts/test.lua"));
	mObject->AddComponent(Cheezy::CreateRef<Cheezy::BoxCollider2DComponent>(glm::vec2(1.0f, 1.0f), true));
	mObject->AddComponent(Cheezy::CreateRef<Cheezy::RigidBodyComponent>());
	mObject->SetShape(new Cheezy::Quad(glm::vec3(0.0f), 0.0f, glm::vec2(1.0f), {0.6f, 0.1f, 0.8f, 1.0f}, m_CheckerboardTexture));
	m_Scene.AddObject(mObject);

	mObject = Cheezy::CreateRef<Cheezy::CheezyObject>();
	mObject->AddComponent(Cheezy::CreateRef<Cheezy::Transform2DComponent>(glm::vec3(), glm::vec2(1.0f), 90.0f));
	mObject->AddComponent(Cheezy::CreateRef<Cheezy::BoxCollider2DComponent>(glm::vec2(1.0f, 1.0f)));
	mObject->SetShape(new Cheezy::Quad(glm::vec3(0.0f), 0.0f, glm::vec2(1.0f), { 0.6f, 0.1f, 0.9f, 1.0f}));
	m_Scene.AddObject(mObject);
	
}

void Sandbox2D::OnDetach()
{
	CZ_PROFILE_FUNCTION();

}

void Sandbox2D::OnFixedUpdate()
{
	CZ_PROFILE_FUNCTION();

	m_Scene.OnFixedUpdate();
}

void Sandbox2D::OnUpdate(Cheezy::Timestep ts)
{
	m_LastTimeStep = ts;

	CZ_PROFILE_FUNCTION();

	m_Scene.OnUpdate(ts);

	Cheezy::Renderer2D::ResetStats();
	{
		CZ_PROFILE_SCOPE("Renderer Prep");
		Cheezy::RenderCommand::SetClearColor(glm::vec4{ 0.1, 0.1, 0.1, 1 });
		Cheezy::RenderCommand::Clear();
	}

	{
		CZ_PROFILE_SCOPE("Renderer Draw");

		Cheezy::Renderer2D::BeginScene(m_Scene);
		Cheezy::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(Cheezy::Event& e)
{
	m_Scene.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
	CZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	if (ImGui::CollapsingHeader("Square Settings"))
	{
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::DragFloat("Square Rotation", &m_SquareRotation, 0.2f, -180.0f, 180.0f);
	}

	if (ImGui::CollapsingHeader("Renderer2D Stats"))
	{
		auto stats{ Cheezy::Renderer2D::GetStats() };
		ImGui::Text("FPS: %f", ((float)1 / m_LastTimeStep));
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	}
	ImGui::End();
}
