#pragma once

#include "Cheezy/Core/Components/CheezyComponent.h"
#include "Cheezy/Core/Components/Transform2DComponent.h"
#include "Cheezy/Core/Components/BoxCollider2DComponent.h"

#include <glm/glm.hpp>

namespace Cheezy
{
	class RigidBodyComponent : public CheezyComponent
	{
	public:
		virtual bool IsUniqueComponent() const override { return true; }
		virtual std::string GetComponentName() const override { return "RidigBody"; }

		virtual void OnFixedUpdate() override;
		virtual void OnUpdate(Timestep ts) override;

		glm::vec2 GetVelocity() { return m_Velocity; }

		void AddForce(glm::vec2 force);

		virtual void OnCollision(Collision2D collision) override;
		virtual void OnCollisionEnter(Collision2D collision) override;
		virtual void OnCollisionExit(Collision2D collision) override;

		virtual void Init() override;

	private:
		glm::vec2 m_Velocity = glm::vec2(0.0f);
		float m_Drag = 0.98f;

		Ref<Transform2DComponent> m_Transform;
		bool m_UseGravity = false;

		std::vector<Ref<BoxCollider2DComponent>> m_CurrentCollisions;
	};
}