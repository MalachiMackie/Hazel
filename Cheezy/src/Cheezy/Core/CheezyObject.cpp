#include "hzpch.h"
#include "CheezyObject.h"

#include "Cheezy/Core/Shapes/Quad.h"

#include <algorithm>
#include <typeinfo>

namespace Cheezy
{
	CheezyObject::CheezyObject()
		: m_Shape(CreateRef<Quad>(glm::vec3(0.0f), 0.0f, glm::vec2(0.0f)))
	{
	}

	void CheezyObject::OnUpdate(Timestep ts)
	{
		for (Ref<CheezyComponent>& component : m_Components)
			component->OnUpdate(ts);
	}
}
