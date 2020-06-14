#include "hzpch.h"
#include "CameraComponent.h"

#include <glm\ext\matrix_projection.hpp>
#include <glm\ext\matrix_transform.hpp>

namespace Cheezy
{

	CameraComponent::CameraComponent(const glm::vec2& dimensions)
		: m_Dimensions(dimensions),
		m_Camera(CreateScope<OrthographicCamera>(-(dimensions.x / dimensions.y), (dimensions.x / dimensions.y), -1, 1))
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

	glm::vec2 CameraComponent::CameraToWorldSpace(const glm::vec2& point) const
	{
		auto transform = m_CheezyObject->GetComponent<Transform2DComponent>()->GetTransform();

		glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.0f), transform.Rotation, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(transform.Scale, 1.0f));

		const glm::vec2& dimensions = GetDimensions();

		glm::vec3 worldPoint = glm::unProject(
			glm::vec3(point, 0.0f),
			modelMatrix,
			GetCamera()->GetProjectionMatrix(),
			glm::vec4(0, 0, dimensions.x, dimensions.y));

		return glm::vec2(worldPoint.x + transform.Position.x, (0 - worldPoint.y) + transform.Position.y);
	}
}

