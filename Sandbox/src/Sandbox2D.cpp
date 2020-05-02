#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

using namespace Cheezy;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox 2D"),
	m_Scene(CreateRef<Scene2D>(OrthographicCameraController{ 1280.0f / 720.0f, true }))
{
}

void Sandbox2D::OnAttach()
{
	CZ_PROFILE_FUNCTION();

	m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
	m_ChernoLogoTexture = Texture2D::Create("assets/textures/ChernoLogo.png");

	auto mObject = CreateRef<CheezyObject>();
	mObject->AddComponent(CreateRef<Transform2DComponent>(Transform2D{ glm::vec3(2.0f, 0.0f, 0.0f) }));
	mObject->AddComponent(CreateRef<ScriptComponent>("src/scripts/test.lua"));
	mObject->AddComponent(CreateRef<BoxCollider2DComponent>());
	mObject->AddComponent(CreateRef<RigidBodyComponent>());
	mObject->SetShape(CreateRef<Quad>(glm::vec3(0.0f), 0.0f, glm::vec2(1.0f), glm::vec4(0.6f, 0.1f, 0.8f, 1.0f), m_CheckerboardTexture));
	m_Scene->AddObject(mObject);

	const Ref<CheezyObject>& mObject2 = CreateRef<CheezyObject>();
	mObject2->AddComponent(CreateRef<Transform2DComponent>(Transform2D{ glm::vec3(0.0f), glm::vec2(1.0f), 45.0f }));
	mObject2->AddComponent(CreateRef<BoxCollider2DComponent>());
	mObject2->SetShape(CreateRef<Quad>(glm::vec3(0.0f), 0.0f, glm::vec2(1.0f), glm::vec4(0.6f, 0.1f, 0.9f, 1.0f)));
	m_Scene->AddObject(mObject2);

	const Ref<CheezyObject>& floor = CreateRef<CheezyObject>();
	floor->AddComponent(CreateRef<Transform2DComponent>(Transform2D{ glm::vec3(0.0f, -3.0f, 0.0f), glm::vec2(10.0f, 1.0f) }));
	floor->AddComponent(CreateRef<BoxCollider2DComponent>());
	floor->SetShape(CreateRef<Quad>(glm::vec3(0.0f), 0.0f, glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), m_CheckerboardTexture));
	m_Scene->AddObject(floor);
	
	Application::Get()->SetScene(m_Scene);
}

void Sandbox2D::OnDetach()
{
	CZ_PROFILE_FUNCTION();

}

void Sandbox2D::OnFixedUpdate()
{
	CZ_PROFILE_FUNCTION();

	m_Scene->OnFixedUpdate();
}

void Sandbox2D::OnUpdate(Timestep ts)
{
	m_LastTimeStep = ts;

	CZ_PROFILE_FUNCTION();

	m_Scene->OnUpdate(ts);

	Renderer2D::ResetStats();
	{
		CZ_PROFILE_SCOPE("Renderer Prep");
		RenderCommand::SetClearColor(glm::vec4{ 0.1, 0.1, 0.1, 1 });
		RenderCommand::Clear();
	}

	{
		CZ_PROFILE_SCOPE("Renderer Draw");

		Renderer2D::BeginScene(m_Scene);
		Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(Event& e)
{
	m_Scene->OnEvent(e);
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
		auto stats{ Renderer2D::GetStats() };
		ImGui::Text("FPS: %f", 1.0f / m_LastTimeStep);
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	}
	ImGui::End();
}
