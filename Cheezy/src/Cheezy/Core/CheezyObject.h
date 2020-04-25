#pragma once

#include "Cheezy/Core/Components/CheezyComponent.h"
#include "Cheezy/Core/Components/Transform2DComponent.h"
#include "Cheezy/Core/Timestep.h"
#include "Cheezy/Core/Shapes/Shape2D.h"

#include <vector>
#include <memory>
#include <utility>
#include <typeinfo>

namespace Cheezy
{
	class CheezyObject : public std::enable_shared_from_this<CheezyObject>
	{
	public:
		CheezyObject();

		void OnUpdate(Timestep ts);

		template<typename T>
		void AddComponent(Ref<T> component)
		{
			static_assert(std::is_base_of<CheezyComponent, T>::value, "the passed component isn't a Cheezy Component");
			if (component->IsUniqueComponent()
				&& std::any_of(m_Components.begin(), m_Components.end(),
					[&component](Ref<CheezyComponent> const& existingComponent) { return typeid(existingComponent.get()) == typeid(component.get()); }))
			{
				CZ_ERROR("A(n) {0} already exists on this object");
				return;
			}

			component->m_CheezyObject = Get();
			component->Init();

			m_Components.push_back(component);
		}

		template<typename T>
		const Ref<T> GetComponent() const
		{
			static_assert(std::is_base_of<CheezyComponent, T>::value, "The passed type is not a cheezy component.");
			for (const auto& existing : m_Components)
			{
				Ref<T> castComponent = std::dynamic_pointer_cast<T>(existing);
				if (castComponent)
				{
					return castComponent;
				}
			}

			return nullptr;
		}

		const Ref<Shape2D>& GetShape() const { return m_Shape; }

		template<typename T>
		bool ContainsComponent()
		{
			return std::any_of(m_Components.begin(), m_Components.end(),
				[](const Ref<CheezyComponent>& component) { return typeid(T) == typeid(*component.get()); });
		}

		template<typename T>
		void SetShape(T* shape)
		{
			static_assert(std::is_base_of<Shape2D, T>::value, "The passed shape isn't a shape2D");
			m_Shape.reset(shape);
		}

		Ref<CheezyObject> Get() { return shared_from_this(); }

	private:
		std::vector<Ref<CheezyComponent>> m_Components;
		Ref<Shape2D> m_Shape;
	};
}