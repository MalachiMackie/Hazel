#pragma once

#include "Cheezy/Core/Shapes/Shape2D.h"
#include "Cheezy/Renderer/Texture.h"
#include "Cheezy/Renderer/Renderer2D.h"

namespace Cheezy
{
	class Quad : public Shape2D
	{
	public:
		Quad(
			const glm::vec3& position = glm::vec3(0.0f),
			float rotation = 0.0f,
			const glm::vec2& scale = glm::vec2(1.0f),
			Ref<Texture2D> texture = Renderer2D::WhiteTexture,
			const glm::vec4& color = glm::vec4(1.0f));

		~Quad() override = default;

		virtual const glm::vec3& GetPosition() const { return m_Position; }
		virtual void SetPosition(const glm::vec3& position) { m_Position = position; }

		virtual float GetRotation() const { return m_Rotation; }
		virtual void SetRotation(float rotation) { m_Rotation = rotation; }

		virtual const glm::vec2& GetSize() const { return m_Size; }
		virtual void SetSize(const glm::vec2& size) { m_Size = size; }

		virtual Ref<Texture2D> GetTexture() const override { return m_Texture; }
		virtual void SetTexture(Ref<Texture2D>& texture) override { m_Texture = texture; }

		virtual const glm::vec4& GetColor() const { return m_Color; }
		virtual void SetColor(const glm::vec4& color) { m_Color = color; }

		virtual uint32_t GetIndexCount() const override { return 4; }
		virtual const std::vector<glm::vec2>& GetVertexPositions() const override { return m_VertexPositions; }
		virtual const std::vector<glm::vec2>& GetTextureCoords() const override { return m_TextureCoords; }

	private:
		glm::vec3 m_Position;
		float m_Rotation;
		glm::vec2 m_Size;

		glm::vec4 m_Color;

		Ref<Texture2D> m_Texture;

		const std::vector<glm::vec2> m_VertexPositions = {
			{ -0.5f, -0.5f },
			{  0.5f, -0.5f },
			{  0.5f,  0.5f },
			{ -0.5f,  0.5f }
		};

		const std::vector<glm::vec2> m_TextureCoords =
		{
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};
	};
}