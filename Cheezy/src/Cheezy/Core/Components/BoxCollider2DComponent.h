#pragma once

#include "Cheezy/Core/Components/CheezyComponent.h"

#include <glm/glm.hpp>

namespace Cheezy
{
	class BoxCollider2DComponent : public CheezyComponent
	{
	private:
		static constexpr int MaxVertices = 5;

	public:
		BoxCollider2DComponent(const glm::vec2& dimensions);

		virtual bool IsUniqueComponent() const override { return true; }
		virtual std::string GetComponentName() const override { return "BoxCollider2D"; }

		virtual void OnUpdate(Timestep ts) override;

		const glm::vec2& GetDimensions() const { return m_Dimensions; }
		glm::vec2 GetVertex(int index) const { CZ_ASSERT(index < MaxVertices, "Index must be less than the max vertices"); return m_Vertices[index]; }
		const std::array<glm::vec2, MaxVertices + 1>& GetVertices() { return m_Vertices; }

	private:
		glm::vec2 m_Dimensions;
		//list of vertices - First one is the pivot point
		std::array<glm::vec2, MaxVertices + 1> m_Vertices;
		
	};
}