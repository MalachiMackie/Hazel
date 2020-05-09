#pragma once

#include "Cheezy.h"

using namespace Cheezy;

class CameraScript : public ScriptComponent
{
public:
	void OnStart() override
	{
		std::vector<Ref<CheezyObject>> allObjects = Application::Get()->GetScene()->GetObjects();
		for (Ref<CheezyObject>& obj : allObjects)
		{
			if (obj->GetTag() == "Player")
			{
				m_FollowTransform = obj->GetComponent<Transform2DComponent>();
				break;
			}
		}
		m_Transform = m_CheezyObject->GetComponent<Transform2DComponent>();
		m_Camera = m_CheezyObject->GetComponent<CameraComponent>();
		m_Camera->GetCamera()->SetProjection(-(16.0f / 9.0f) * m_ZoomLevel, (16.0f / 9.0f) * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void OnFixedUpdate() override
	{
		m_Transform->SetPosition(glm::mix(m_Transform->GetPosition(), m_FollowTransform->GetPosition(), 0.1f));
	}

	void OnUpdate(Timestep ts) override
	{
	}

private:
	Ref<CameraComponent> m_Camera;
	Ref<Transform2DComponent> m_Transform;
	Ref<Transform2DComponent> m_FollowTransform;
	float m_ZoomLevel = 4.0f;
};