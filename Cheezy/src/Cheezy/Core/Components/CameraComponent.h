#pragma once

#include "Cheezy/Renderer/OrthographicCamera.h"
#include "Cheezy/Core/Components/CheezyComponent.h"
#include "Cheezy/Core/Components/Transform2DComponent.h"

namespace Cheezy
{
	class CameraComponent : public CheezyComponent
	{
	public:
		CameraComponent(OrthographicCamera camera);
		CameraComponent(float aspectRatio);

		const Scope<OrthographicCamera>& GetCamera() const { return m_Camera; }

		virtual bool IsUniqueComponent() const override { return true; }
		virtual std::string GetComponentName() const override { return "Camera"; }

		virtual void OnUpdate(Timestep ts) override;

	private:
		Scope<OrthographicCamera> m_Camera;
		Ref<Transform2DComponent> m_TransformComponent;
	};
}