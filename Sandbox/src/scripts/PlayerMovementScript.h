#pragma once

#include "Cheezy.h"

using namespace Cheezy;

class PlayerMovementScript : public ScriptComponent
{
	void OnStart() override
	{
		m_Rigidbody = m_CheezyObject->GetComponent<RigidBodyComponent>();
	}

	void OnFixedUpdate() override
	{
		glm::vec2 movementForce = glm::vec2(0.0f);

		if (Input::IsKeyPressed(CZ_KEY_A))
			movementForce -= glm::vec2(1.0f, 0.0f);
		if (Input::IsKeyPressed(CZ_KEY_D))
			movementForce += glm::vec2(1.0f, 0.0f);
		if (Input::IsKeyPressed(CZ_KEY_W))
			movementForce += glm::vec2(0.0f, 1.0f);
		if (Input::IsKeyPressed(CZ_KEY_S))
			movementForce -= glm::vec2(0.0f, 1.0f);

		const glm::vec2& velocity = m_Rigidbody->GetVelocity();
		if (movementForce != glm::vec2(0.0f) && glm::length(velocity) < m_MaxSpeed)
		{
			m_Rigidbody->AddForce(glm::normalize(movementForce) * m_MoveForce);
		}
	}

private:
	Ref<RigidBodyComponent> m_Rigidbody;
	float m_MaxSpeed = 7.5f;
	float m_MoveForce = 1.0f;
};