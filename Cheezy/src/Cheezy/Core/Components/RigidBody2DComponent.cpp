#include "hzpch.h"
#include "RigidBody2DComponent.h"

#include "Cheezy/Core/Application.h"

#include <glm/geometric.hpp>

namespace Cheezy
{
	void RigidBodyComponent::OnCollisionEnter(const Collision2D& collision)
	{
		m_CurrentCollisions.push_back(collision.OtherCollider);
	}

	void RigidBodyComponent::OnCollisionExit(const Collision2D& collision)
	{
		auto& found = std::find_if(m_CurrentCollisions.begin(), m_CurrentCollisions.end(),
			[&collision](Ref<BoxCollider2DComponent>& currentCol) { return currentCol == collision.OtherCollider; });
		if (found != m_CurrentCollisions.end())
			m_CurrentCollisions.erase(found);
	}

	void RigidBodyComponent::OnTriggerEnter(const Collision2D& collision)
	{
		m_CurrentTriggers.push_back(collision.OtherCollider);
	}

	void RigidBodyComponent::OnTriggerExit(const Collision2D& collision)
	{
		auto& found = std::find_if(m_CurrentTriggers.begin(), m_CurrentTriggers.end(),
			[&collision](Ref<BoxCollider2DComponent>& currentCol) { return currentCol == collision.OtherCollider; });
		if (found != m_CurrentTriggers.end())
			m_CurrentTriggers.erase(found);
	}

	void RigidBodyComponent::Init()
	{
		m_Transform = m_CheezyObject->GetComponent<Transform2DComponent>();
	}

	void RigidBodyComponent::OnFixedUpdate()
	{
		m_Velocity *= m_Drag;

		if (glm::length(m_Velocity) <= 0.01f)
		{
			m_Velocity = glm::vec2(0.0f);
		}

		if (m_UseGravity)
			m_Velocity += (Application::GetGravity() * Application::GetFixedFrameTime());

		const glm::vec3& prevPosition = m_Transform->GetPosition();
		glm::vec3 newPosition = prevPosition;

		newPosition += (glm::vec3(m_Velocity, 0.0f) * 0.02f);

		std::vector<Collision2D>& possibleCollisions = BoxCollider2DComponent::CheckCollisionForObjectWithTransform(
			m_CheezyObject,
			{ newPosition, m_Transform->GetScale(), m_Transform->GetRotation() });

		for (Collision2D& collision : possibleCollisions)
		{
			newPosition += glm::vec3(collision.PushVector, 0.0f);
			Ref<CheezyObject>& otherObject = collision.OtherCollider->GetCheezyObject();

			Collision2D otherCollision{ collision.PushVector * -1.0f, m_CheezyObject->GetComponent<BoxCollider2DComponent>() };

			if (!std::any_of(m_CurrentCollisions.begin(), m_CurrentCollisions.end(),
				[&collision](const Ref<BoxCollider2DComponent>& otherCollider)
			{
				return collision.OtherCollider.get() == otherCollider.get();
			}))
			{
				m_CheezyObject->OnCollisionEnter(collision);
				otherObject->OnCollisionEnter(otherCollision);
			}

			m_CheezyObject->OnCollision(collision);
			otherObject->OnCollision(otherCollision);
		}

		for (Ref<BoxCollider2DComponent>& existingCollider : m_CurrentCollisions)
		{
			Ref<CheezyObject>& otherObject = existingCollider->GetCheezyObject();
			Collision2D collision{ glm::vec3(0.0f), existingCollider };
			Collision2D otherCollision{ glm::vec3(0.0f), m_CheezyObject->GetComponent<BoxCollider2DComponent>() };

			if (!std::any_of(possibleCollisions.begin(), possibleCollisions.end(),
				[&existingCollider](Collision2D& collision)
			{
				return existingCollider.get() == collision.OtherCollider.get();
			}))
			{
				m_CheezyObject->OnCollisionExit(collision);
				otherObject->OnCollisionExit(otherCollision);
			}
		}

		m_Velocity = (newPosition - prevPosition) / 0.02f;

		m_Transform->SetPosition(newPosition);
	}

	void RigidBodyComponent::OnUpdate(Timestep ts)
	{
	}

	void RigidBodyComponent::AddForce(glm::vec2 force)
	{
		m_Velocity += force;
	}
}

