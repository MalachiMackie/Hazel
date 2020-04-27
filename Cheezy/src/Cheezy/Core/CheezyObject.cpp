#include "hzpch.h"
#include "CheezyObject.h"

#include "Cheezy/Core/Shapes/Quad.h"
#include "Cheezy/Core/Components/CheezyComponent.h"

#include <algorithm>
#include <typeinfo>

namespace Cheezy
{
	CheezyObject::CheezyObject()
		: m_Shape(CreateRef<Quad>(glm::vec3(0.0f), 0.0f, glm::vec2(0.0f)))
	{
	}

	void CheezyObject::OnFixedUpdate()
	{
		for (Ref<CheezyComponent>& component : m_Components)
			component->OnFixedUpdate();
	}

	void CheezyObject::OnUpdate(Timestep ts)
	{
		for (Ref<CheezyComponent>& component : m_Components)
			component->OnUpdate(ts);
	}

	void CheezyObject::OnCollision(Collision2D collision)
	{
		for (Ref<CheezyComponent>& component : m_Components)
			component->OnCollision(collision);
	}

	void CheezyObject::OnCollisionEnter(Collision2D collision)
	{
		for (Ref<CheezyComponent>& component : m_Components)
			component->OnCollisionEnter(collision);
	}

	void CheezyObject::OnCollisionExit(Collision2D collision)
	{
		for (Ref<CheezyComponent>& component : m_Components)
			component->OnCollisionExit(collision);
	}
}
