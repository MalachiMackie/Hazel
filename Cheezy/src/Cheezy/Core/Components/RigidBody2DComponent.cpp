#include "hzpch.h"
#include "RigidBody2DComponent.h"

#include "Cheezy/Core/Application.h"

#include <glm/geometric.hpp>

namespace Cheezy
{
	void RigidBodyComponent::OnCollision(Collision2D collision)
	{
		auto& found = std::find_if(m_CurrentCollisions.begin(), m_CurrentCollisions.end(),
			[&collision](Ref<BoxCollider2DComponent>& currentCol) { return currentCol == collision.OtherCollider; });
		if (found != m_CurrentCollisions.end())
		{
			m_CurrentCollisions.erase(found);
			m_CurrentCollisions.push_back(collision.OtherCollider);
		}
	}

	void RigidBodyComponent::OnCollisionEnter(Collision2D collision)
	{
		m_CurrentCollisions.push_back(collision.OtherCollider);
	}

	void RigidBodyComponent::OnCollisionExit(Collision2D collision)
	{
		auto& found = std::find_if(m_CurrentCollisions.begin(), m_CurrentCollisions.end(), 
			[&collision](Ref<BoxCollider2DComponent>& currentCol) { return currentCol == collision.OtherCollider; });
		if (found != m_CurrentCollisions.end())
			m_CurrentCollisions.erase(found);
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

