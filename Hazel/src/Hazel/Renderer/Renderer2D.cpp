#include "hzpch.h"
#include "Renderer2D.h"

#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVA;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();
		s_Data->QuadVA = VertexArray::Create();

		float squareVerts[4 * 3]
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		auto squareVB = Ref<VertexBuffer>(VertexBuffer::Create(sizeof(squareVerts), squareVerts));

		BufferLayout squareLayout = {
			{ ShaderDataType::Float3, "a_Position" },
		};

		squareVB->SetLayout(squareLayout);
		s_Data->QuadVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		auto squareIB = Ref<IndexBuffer>(IndexBuffer::Create(sizeof(squareIndices) / sizeof(uint32_t), squareIndices));
		s_Data->QuadVA->SetIndexBuffer(squareIB);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, dimensions, color, 0.0f);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& color, float rotation)
	{
		DrawQuad({position.x, position.y, 0.0f}, dimensions, color, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& dimensions, const glm::vec4& color)
	{
		DrawQuad(position, dimensions, color, 0);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& dimensions, const glm::vec4& color, float rotation)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), {dimensions.x, dimensions.y, 1.0f})
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f});
		 
		s_Data->FlatColorShader->SetMat4("u_Transform", transform);
		s_Data->QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVA);
	}
}
