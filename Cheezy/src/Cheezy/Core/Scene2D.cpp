#include "hzpch.h"
#include "Scene2D.h"

#include "Cheezy/Core/Components/BoxCollider2DComponent.h"
#include "Cheezy/Core/Components/RigidBody2DComponent.h"

#include <future>

namespace Cheezy
{
	void Scene2D::OnFixedUpdate()
	{
		for (Ref<CheezyObject>& obj : m_Objects)
			obj->OnFixedUpdate();
	}

	void Scene2D::OnUpdate(Timestep ts)
	{
		CZ_PROFILE_FUNCTION();
		for (Ref<CheezyObject>& obj : m_Objects)
			obj->OnUpdate(ts);
	}

	void Scene2D::OnEvent(Event& e)
	{
	}

	void Scene2D::OnStart()
	{
		CZ_PROFILE_FUNCTION();
		for (Ref<CheezyObject>& obj : m_Objects)
			obj->OnStart();
	}
}