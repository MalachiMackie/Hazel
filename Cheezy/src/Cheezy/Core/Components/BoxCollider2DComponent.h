#pragma once

#include "Cheezy/Core/Components/CheezyComponent.h"
#include "Cheezy/Core/Collision2D.h"

#include <glm/glm.hpp>

namespace Cheezy
{
	class BoxCollider2DComponent : public CheezyComponent
	{
	private:
		static constexpr int MaxVertices = 4;

	public:
		BoxCollider2DComponent(const glm::vec2& dimensions, bool isDynamic = false);

		virtual bool IsUniqueComponent() const override { return true; }
		virtual std::string GetComponentName() const override { return "BoxCollider2D"; }

		virtual void OnUpdate(Timestep ts) override;

		virtual void OnCollision(Collision2D collision) override;
		virtual void OnCollisionEnter(Collision2D collision) override;
		virtual void OnCollisionExit(Collision2D collision) override;

		const glm::vec2& GetDimensions() const { return m_Dimensions; }
		glm::vec2 GetVertex(int index) const { CZ_ASSERT(index < MaxVertices, "Index must be less than the max vertices"); return m_Vertices[index]; }
		const std::array<glm::vec2, MaxVertices>& GetVertices() const { return m_Vertices; }

		std::vector<Ref<BoxCollider2DComponent>> GetCollidingWith() const { return m_CollidingWith; }

		bool GetIsDynamic() const { return m_IsDynamic; }

		static std::pair<bool, glm::vec2> CheckCollisionBetweenColliders(const Ref<BoxCollider2DComponent>& first, const Ref<BoxCollider2DComponent>& second);
		static void CheckCollisionsForObjects(std::vector<Ref<CheezyObject>> cheezyObjects);
		static void CheckCollisionsForObject(Ref<CheezyObject>* cheezyObject, std::vector<Ref<CheezyObject>> otherObjects, int index);

	private:
		glm::vec2 m_Dimensions;
		//list of vertices
		std::array<glm::vec2, MaxVertices> m_Vertices;
		bool m_IsDynamic;
		std::vector<Ref<BoxCollider2DComponent>> m_CollidingWith;

		static std::pair<bool, glm::vec2> NoRotationCollisionCheck(const Ref<BoxCollider2DComponent>& first, const Ref<BoxCollider2DComponent>& second);
		static std::pair<bool, glm::vec2> RotationCollisionCheck(const Ref<BoxCollider2DComponent>& first, const Ref<BoxCollider2DComponent>& second);
		
	};
}