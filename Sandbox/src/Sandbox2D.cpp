#include "Sandbox2D.h"

#include "Scripts/CameraScript.h"
#include "Scripts/FloorScript.h"
#include "Scripts/PlayerMovementScript.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

using namespace Cheezy;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox 2D"),
	m_Scene(CreateRef<Scene2D>())
{
}

void Sandbox2D::OnAttach()
{
	CZ_PROFILE_FUNCTION();

	m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
	m_ChernoLogoTexture = Texture2D::Create("assets/textures/ChernoLogo.png");

	//camera object
	auto& cameraObject = CreateRef<CheezyObject>();
	cameraObject->AddComponent(CreateRef<Transform2DComponent>());
	cameraObject->AddComponent(CreateRef<CameraComponent>(glm::vec2(1280.0f, 720.0f)));
	cameraObject->AddComponent(CreateRef<CameraScript>());
	m_Scene->AddObject(cameraObject);

	//Player
	auto& playerObj = CreateRef<CheezyObject>();
	playerObj->SetTag("Player");
	playerObj->AddComponent(CreateRef<Transform2DComponent>(Transform2D{ glm::vec3(2.0f, 0.0f, 0.0f) }));
	playerObj->AddComponent(CreateRef<PlayerMovementScript>());
	playerObj->AddComponent(CreateRef<BoxCollider2DComponent>());
	playerObj->AddComponent(CreateRef<RigidBodyComponent>());
	playerObj->SetShape(CreateRef<Quad>(glm::vec3(0.0f), 0.0f, glm::vec2(1.0f), glm::vec4(0.6f, 0.1f, 0.8f, 1.0f), m_CheckerboardTexture));
	m_Scene->AddObject(playerObj);

	//Floor Piece
	auto& floorObj = CreateRef<CheezyObject>();
	floorObj->AddComponent(CreateRef<Transform2DComponent>(Transform2D{ glm::vec3(-1.0f, 0.0f, 0.0f)}));
	floorObj->AddComponent(CreateRef<FloorScript>());
	floorObj->AddComponent(CreateRef<BoxCollider2DComponent>(true));
	floorObj->SetShape(CreateRef<Quad>(glm::vec3(0.0f), 0.0f, glm::vec2(1.0f), glm::vec4(0.15f, 0.5f, 0.5f, 0.7f)));
	m_Scene->AddObject(floorObj);
	
	Application::Get()->SetScene(m_Scene);
}

void Sandbox2D::OnStart()
{
	m_Scene->OnStart();
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
