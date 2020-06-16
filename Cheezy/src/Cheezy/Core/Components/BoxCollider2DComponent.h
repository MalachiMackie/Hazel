#pragma once

#include "Cheezy/Core/Components/CheezyComponent.h"
#include "Cheezy/Core/Collision2D.h"
#include "Cheezy/Core/Transform2D.h"


#include <glm/glm.hpp>

namespace Cheezy
{
	class BoxCollider2DComponent : public CheezyComponent, public std::enable_shared_from_this<BoxCollider2DComponent>
	{
	private:
		static constexpr int MaxVertices = 4;

	public:
		BoxCollider2DComponent(bool isTrigger = false, const glm::vec2& dimensions = glm::vec2(1.0f), const char* tag = "");

		virtual bool IsUniqueComponent() const override { return true; }
		virtual std::string GetComponentName() const override { return "BoxCollider2D"; }

		virtual void OnUpdate(Timestep ts) override;

		virtual void OnTriggerEnter(const Collision2D& collision) override;
		virtual void OnTriggerExit(const Collision2D& collision) override;

		const glm::vec2& GetDimensions() const { return m_Dimensions; }
		const glm::vec2& GetVertex(int index) const { CZ_ASSERT(index < MaxVertices, "Index must be less than the max vertices"); return m_Vertices[index]; }
		const std::array<glm::vec2, MaxVertices>& GetVertices() const { return m_Vertices; }
		const char* GetTag() const { return m_Tag; }
		bool IsTrigger() { return m_IsTrigger; }

		Ref<BoxCollider2DComponent> Get() { return shared_from_this(); }

		//static collision detection
	public:
		static std::pair<bool, glm::vec2> CheckCollisionBetweenColliders(
			const Ref<BoxCollider2DComponent>& collider1, const Transform2D& transform1,
			const Ref<BoxCollider2DComponent>& collider2, const Transform2D& transform2);
		static void CheckTriggerCollisions(const std::vector<Ref<CheezyObject>>& cheezyObjects);
		static void CheckTriggersForObject(const Ref<CheezyObject>* cheezyObject, const std::vector<Ref<CheezyObject>>& otherObjects, int index);

		static std::vector<Collision2D> CheckCollisionForObjectWithTransform(const Ref<CheezyObject>& cheezyObject, const Transform2D& transform);

	private:
		glm::vec2 m_Dimensions;
		//list of vertices
		std::array<glm::vec2, MaxVertices> m_Vertices;
		std::vector<Ref<BoxCollider2DComponent>> m_CurrentTriggers;
		const char* m_Tag;

		bool m_IsTrigger = false;
		bool m_MouseHovering = false;
		
	};
}