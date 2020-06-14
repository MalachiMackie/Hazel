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

	void CheezyObject::OnStart()
	{
		for (Ref<CheezyComponent>& component : m_Components)
			component->OnStart();
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

	void CheezyObject::OnCollision(const Collision2D& collision)
	{
		for (Ref<CheezyComponent>& component : m_Components)
			component->OnCollision(collision);
	}

	void CheezyObject::OnCollisionEnter(const Collision2D& collision)
	{
		for (Ref<CheezyComponent>& component : m_Components)
			component->OnCollisionEnter(collision);
	}

	void CheezyObject::OnCollisionExit(const Collision2D& collision)
	{
		for (Ref<CheezyComponent>& component : m_Components)
			component->OnCollisionExit(collision);
	}

	void CheezyObject::OnEvent(Event& e)
	{
		for (Ref<CheezyComponent>& component : m_Components)
			component->OnEvent(e);
	}

	void CheezyObject::OnMouseEnter()
	{
		for (Ref<CheezyComponent>& component : m_Components)
			component->OnMouseEnter();
	}

	void CheezyObject::OnMouseHover()
	{
		for (Ref<CheezyComponent>& component : m_Components)
			component->OnMouseHover();
	}

	void CheezyObject::OnMouseExit()
	{
		for (Ref<CheezyComponent>& component : m_Components)
			component->OnMouseExit();
	}
}
