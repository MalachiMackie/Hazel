#include "hzpch.h"
#include "Quad.h"

#include "Cheezy/Renderer/Renderer2D.h"

namespace Cheezy
{
	Quad::Quad(const glm::vec3& position, float rotation, const glm::vec2& scale, Ref<Texture2D> texture, const glm::vec4& color)
		: m_Position(position), m_Rotation(rotation), m_Size(scale), m_Texture(texture), m_Color(color)
	{
		
	}

}

