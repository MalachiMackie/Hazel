#include "hzpch.h"
#include "Renderer2D.h"

#include "Cheezy/Renderer/VertexArray.h"
#include "Cheezy/Renderer/Shader.h"
#include "Cheezy/Renderer/RenderCommand.h"

#include "Cheezy/Core/Components/Transform2DComponent.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cheezy
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoords;
		float TextureIndex = 0.0f; // default white texture
		float TilingFactor = 1.0f;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVA;
		Ref<VertexBuffer> QuadVB;
		Ref<Shader> TextureShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 == white texture

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	Ref<Texture2D> Renderer2D::WhiteTexture = nullptr;

	void Renderer2D::Init()
	{
		CZ_PROFILE_FUNCTION();

		s_Data.QuadVA = VertexArray::Create();

		s_Data.QuadVB = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		s_Data.QuadVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TextureCoords" },
			{ ShaderDataType::Float, "a_TextureIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" }
			});
		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		auto quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVA->SetIndexBuffer(quadIB);

		delete[] quadIndices;

		WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = WhiteTexture;

		s_Data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f};
		s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f};
		s_Data.QuadVertexPositions[3] = {-0.5f,  0.5f, 0.0f, 1.0f};
	}

	void Renderer2D::Shutdown()
	{
		CZ_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		CZ_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.QuadIndexCount = 0;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const Scene2D& scene)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", scene.GetCameraController().GetCamera().GetViewProjectionMatrix());

		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.QuadIndexCount = 0;

		s_Data.TextureSlotIndex = 1;

		for (const Ref<CheezyObject>& object : scene.GetObjects())
		{
			Ref<Shape2D> shape = object->GetShape();
			if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
				FlushAndReset();

			const Ref<Transform2DComponent> objectTransform = object->GetComponent<Transform2DComponent>();

			glm::vec2 shapeScale = shape->GetSize();

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), shape->GetPosition() + objectTransform->GetPosition());
			transform *= glm::rotate(glm::mat4(1.0f), glm::radians(shape->GetRotation() + objectTransform->GetRotation()), { 0.0f, 0.0f, 1.0f });
			transform *= glm::scale(glm::mat4(1.0f), { shapeScale.x * objectTransform->GetSize().x, shapeScale.y * objectTransform->GetSize().y, 1.0f });

			float textureIndex = -1.0f;

			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i].get() == *shape->GetTexture().get())
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == -1.0f)
			{
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex++] = shape->GetTexture();
			}

			for (uint32_t i = 0; i < shape->GetIndexCount(); i++)
			{
				auto vertexPosition = shape->GetVertexPositions()[i];
				s_Data.QuadVertexBufferPtr->Position = transform * glm::vec4(vertexPosition.x, vertexPosition.y, 0.0f, 1.0f);
				s_Data.QuadVertexBufferPtr->Color = shape->GetColor();
				s_Data.QuadVertexBufferPtr->TextureCoords = shape->GetTextureCoords()[i];
				s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
				s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;

				s_Data.QuadVertexBufferPtr++;
			}
			s_Data.QuadIndexCount += 6;
			s_Data.Stats.QuadCount++;
		}
	}

	void Renderer2D::EndScene()
	{
		CZ_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		CZ_PROFILE_FUNCTION();

		// Bind Textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);

		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.QuadIndexCount = 0;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, dimensions, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& dimensions, const glm::vec4& color)
	{
		CZ_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { dimensions.x, dimensions.y, 1.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& dimensions, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, dimensions, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& dimensions, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		CZ_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex++] = texture;
		}
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { dimensions.x, dimensions.y, 1.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& dimensions, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, dimensions, rotation, texture, tilingFactor, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& dimensions, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		CZ_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex++] = texture;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { dimensions.x, dimensions.y, 1.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& dimensions, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, dimensions, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& dimensions, float rotation, const glm::vec4& color)
	{
		CZ_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { dimensions.x, dimensions.y, 1.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoords = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}



	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}
}
