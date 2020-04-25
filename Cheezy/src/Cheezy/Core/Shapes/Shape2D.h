#pragma once

#include "Cheezy/Renderer/Texture.h"

#include <glm/glm.hpp>
#include <vector>

namespace Cheezy
{
	class Shape2D
	{
	public:
		virtual ~Shape2D() = default;

		virtual const glm::vec3& GetPosition() const = 0;
		virtual void SetPosition(const glm::vec3& position) = 0;

		virtual float GetRotation() const = 0;
		virtual void SetRotation(float rotation) = 0;

		virtual const glm::vec2& GetSize() const = 0;
		virtual void SetSize(const glm::vec2& size) = 0;

		virtual const glm::vec4& GetColor() const = 0;
		virtual void SetColor(const glm::vec4& color) = 0;

		virtual Ref<Texture2D> GetTexture() const = 0;
		virtual void SetTexture(Ref<Texture2D>& texture) = 0;

		virtual uint32_t GetIndexCount() const = 0;

		virtual const std::vector<glm::vec2>& GetVertexPositions() const = 0;
		virtual const std::vector<glm::vec2>& GetTextureCoords() const = 0;
	};
}