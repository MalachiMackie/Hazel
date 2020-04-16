#include "ExampleLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer::ExampleLayer()
	: Layer("Example"),
	m_CameraController(1280.0f / 720.0f), m_SquarePos(0.0f)
{
	m_TriVA = Hazel::VertexArray::Create();

	float vertices[3 * 7]
	{
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	auto triVB = Hazel::Ref<Hazel::VertexBuffer>(Hazel::VertexBuffer::Create(sizeof(vertices), vertices));

	Hazel::BufferLayout layout = {
		{ Hazel::ShaderDataType::Float3, "a_Position" },
		{ Hazel::ShaderDataType::Float4, "a_Color" }
	};

	triVB->SetLayout(layout);
	m_TriVA->AddVertexBuffer(triVB);

	uint32_t indices[3] = { 0, 1, 2 };
	auto triIB = Hazel::Ref<Hazel::IndexBuffer>(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	m_TriVA->SetIndexBuffer(triIB);

	m_SquareVA = Hazel::VertexArray::Create();

	float squareVerts[4 * 5]
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	};

	auto squareVB = Hazel::Ref<Hazel::VertexBuffer>(Hazel::VertexBuffer::Create(sizeof(squareVerts), squareVerts));

	Hazel::BufferLayout squareLayout = {
		{ Hazel::ShaderDataType::Float3, "a_Position" },
		{ Hazel::ShaderDataType::Float2, "a_TexCoord" },
	};

	squareVB->SetLayout(squareLayout);
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	auto squareIB = Hazel::Ref<Hazel::IndexBuffer>(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);

	std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

	std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
	
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

	auto vertexPosColorShader = m_ShaderLibrary.Load("VertexPosColor", vertexSrc, fragmentSrc);
	auto flatColorShader = m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
	auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");

	textureShader->Bind();
	textureShader->SetInt("u_Texture", 0);
}

void ExampleLayer::SquareMovement(Hazel::Timestep ts)
{
	if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
		m_SquarePos.x -= m_SquareMoveSpeed * ts;
	if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
		m_SquarePos.x += m_SquareMoveSpeed * ts;

	if (Hazel::Input::IsKeyPressed(HZ_KEY_W))
		m_SquarePos.y += m_SquareMoveSpeed * ts;
	if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
		m_SquarePos.y -= m_SquareMoveSpeed * ts;
}


void ExampleLayer::OnUpdate(Hazel::Timestep ts)
{
	if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT))
		m_CameraController.OnUpdate(ts);
	else
		SquareMovement(ts);

	Hazel::RenderCommand::SetClearColor(glm::vec4{ 0.1, 0.1, 0.1, 1 });
	Hazel::RenderCommand::Clear();

	Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

	static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	Hazel::Renderer::Submit(m_ShaderLibrary.Get("VertexPosColor"), m_TriVA);

	auto flatColorShader = m_ShaderLibrary.Get("FlatColor");

	flatColorShader->Bind();
	flatColorShader->SetFloat4("u_Color", m_SquareColor);

	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePos + pos) * scale;
			Hazel::Renderer::Submit(flatColorShader, m_SquareVA, transform);
		}
	}

	auto textureShader = m_ShaderLibrary.Get("Texture");

	m_CheckerboardTexture->Bind();
	Hazel::Renderer::Submit(
		textureShader,
		m_SquareVA,
		glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f)));
	m_ChernoLogoTexture->Bind();
	Hazel::Renderer::Submit(textureShader, m_SquareVA,
		glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f)));

	Hazel::Renderer::EndScene();
}

void ExampleLayer::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}