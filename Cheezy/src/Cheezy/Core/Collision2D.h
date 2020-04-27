#pragma once

#include <glm/glm.hpp>

namespace Cheezy
{
	class BoxCollider2DComponent;

	struct Collision2D
	{
	public:
		glm::vec2 PushVector;
		Ref<BoxCollider2DComponent> OtherCollider;
	};
}