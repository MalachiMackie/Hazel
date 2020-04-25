#include "hzpch.h"
#include "Transform2DComponent.h"

namespace Cheezy
{
	Transform2DComponent::Transform2DComponent(const glm::vec3& position, const glm::vec2& size, float rotation)
		: m_Position(position), m_Size(size), m_Rotation(rotation)
	{
	}
}
