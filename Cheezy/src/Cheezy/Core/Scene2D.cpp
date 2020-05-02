#include "hzpch.h"
#include "Scene2D.h"

#include "Cheezy/Core/Components/BoxCollider2DComponent.h"
#include "Cheezy/Core/Components/RigidBody2DComponent.h"

#include <future>

namespace Cheezy
{
	void Scene2D::OnFixedUpdate()
	{
		m_CameraController.OnFixedUpdate();

		for (Ref<CheezyObject>& obj : m_Objects)
			obj->OnFixedUpdate();
	}

	void Scene2D::OnUpdate(Timestep ts)
	{
		CZ_PROFILE_FUNCTION();

		m_CameraController.OnUpdate(ts);

		for (Ref<CheezyObject>& obj : m_Objects)
			obj->OnUpdate(ts);
	}

	void Scene2D::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}