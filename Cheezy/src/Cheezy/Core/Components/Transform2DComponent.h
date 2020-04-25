#pragma once

#include "Cheezy/Core/Components/CheezyComponent.h"

#include <glm/glm.hpp>

constexpr auto CZ_TRANSFORM_2D_COMPONENT = "Transform2DComponent";

namespace Cheezy
{
	class Transform2DComponent : public CheezyComponent
	{
	public:
		Transform2DComponent(
			const glm::vec3& position = glm::vec3(0.0f),
			const glm::vec2& size = glm::vec2(1.0f),
			float rotation = 0.0f);

		virtual bool IsUniqueComponent() const override { return true; }
		virtual std::string GetComponentName() const override { return CZ_TRANSFORM_2D_COMPONENT; };

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; }

		const glm::vec2& GetSize() const { return m_Size; }
		void SetScale(const glm::vec2& scale) { m_Size = scale; }

		const float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; }

	private:
		glm::vec3 m_Position;
		glm::vec2 m_Size;
		float m_Rotation;
	};
}