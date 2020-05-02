#pragma once

#include "Cheezy/Core/Components/CheezyComponent.h"
#include "Cheezy/Core/Transform2D.h"

#include <glm/glm.hpp>

constexpr auto CZ_TRANSFORM_2D_COMPONENT = "Transform2DComponent";

namespace Cheezy
{
	class Transform2DComponent : public CheezyComponent
	{
	public:
		Transform2DComponent(const Transform2D& transform = {});

		virtual bool IsUniqueComponent() const override { return true; }
		virtual std::string GetComponentName() const override { return CZ_TRANSFORM_2D_COMPONENT; };

		const glm::vec3& GetPosition() const { return m_Transform.Position; }
		void SetPosition(const glm::vec3& position) { m_Transform.Position = position; }

		const glm::vec2& GetScale() const { return m_Transform.Scale; }
		void SetScale(const glm::vec2& scale) { m_Transform.Scale = scale; }

		const float GetRotation() const { return m_Transform.Rotation; }
		void SetRotation(float rotation) { m_Transform.Rotation = rotation; }

		const Transform2D& GetTransform() const { return m_Transform; }

	private:
		Transform2D m_Transform;
	};
}