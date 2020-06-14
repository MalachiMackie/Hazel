#pragma once

#include "Cheezy/Renderer/OrthographicCamera.h"
#include "Cheezy/Core/Components/CheezyComponent.h"
#include "Cheezy/Core/Components/Transform2DComponent.h"

namespace Cheezy
{
	class CameraComponent : public CheezyComponent
	{
	public:
		CameraComponent(const glm::vec2& dimensions);

		const Scope<OrthographicCamera>& GetCamera() const { return m_Camera; }

		const glm::vec2& GetDimensions() const { return m_Dimensions; }

		glm::vec2 CameraToWorldSpace(const glm::vec2& point) const;

		virtual bool IsUniqueComponent() const override { return true; }
		virtual std::string GetComponentName() const override { return "Camera"; }

		virtual void OnUpdate(Timestep ts) override;

	private:
		Scope<OrthographicCamera> m_Camera;
		Ref<Transform2DComponent> m_TransformComponent;
		glm::vec2 m_Dimensions;
	};
}