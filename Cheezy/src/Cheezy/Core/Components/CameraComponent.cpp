#include "hzpch.h"
#include "CameraComponent.h"

namespace Cheezy
{
	CameraComponent::CameraComponent(OrthographicCamera camera)
		: m_Camera(CreateScope<OrthographicCamera>(camera))
	{
	}

	CameraComponent::CameraComponent(float aspectRatio)
		: m_Camera(CreateScope<OrthographicCamera>(-aspectRatio, aspectRatio, -1, 1))
	{
	}

	void CameraComponent::OnUpdate(Timestep ts)
	{
		Ref<Transform2DComponent>& transform = m_TransformComponent;
		if (!transform)
		{
			m_TransformComponent = m_CheezyObject->GetComponent<Transform2DComponent>();
			transform = m_TransformComponent;
		}

		const glm::vec3& cameraPosition = m_Camera->GetPosition();
		const glm::vec3& transformPosition = transform->GetPosition();
		if (transformPosition != cameraPosition)
		{
			m_Camera->SetPosition(transformPosition);
		}
	}
}

