#pragma once

#include <glm/glm.hpp>

namespace Cheezy
{
	struct Transform2D
	{
		glm::vec3 Position;
		glm::vec2 Scale;
		float Rotation;

		Transform2D(glm::vec3 position = glm::vec3(0.0f), glm::vec2 scale = glm::vec2(1.0f), float rotation = 0.0f)
			: Position(position), Scale(scale), Rotation(rotation)
		{
		}
	};
}