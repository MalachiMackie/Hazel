#include "hzpch.h"
#include "BoxCollider2DComponent.h"

namespace Cheezy
{
	BoxCollider2DComponent::BoxCollider2DComponent(const glm::vec2& dimensions)
		: m_Dimensions(dimensions), m_Vertices({ glm::vec2(dimensions.x / 2, dimensions.y / 2), {0, 0}, {dimensions.x, 0}, {dimensions.x, dimensions.y}, {0, dimensions.y} })
	{
		
	}

	void BoxCollider2DComponent::OnUpdate(Timestep timestep)
	{

	}
}
