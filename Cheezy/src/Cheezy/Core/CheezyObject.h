#pragma once

#include "Cheezy/Core/Timestep.h"
#include "Cheezy/Core/Shapes/Shape2D.h"
#include "Cheezy/Core/Collision2D.h"

#include <vector>
#include <memory>
#include <utility>
#include <typeinfo>

namespace Cheezy
{
	class CheezyComponent;
	class BoxCollider2DComponent;

	class CheezyObject : public std::enable_shared_from_this<CheezyObject>
	{
	public:
		CheezyObject();

		void OnStart();
		/*
			Runs 50 times per second.
			Will run multiple times per rendered frame if it has to
		*/
		void OnFixedUpdate();
		void OnUpdate(Timestep ts);

		template<typename T>
		void AddComponent(Ref<T>& component)
		{
			static_assert(std::is_base_of<CheezyComponent, T>::value, "the passed component isn't a Cheezy Component");
			if (component->IsUniqueComponent()
				&& std::any_of(m_Components.begin(), m_Components.end(),
					[&component](Ref<CheezyComponent> const& existingComponent) { return typeid(existingComponent.get()) == typeid(component.get()); }))
			{
				CZ_ERROR("A(n) {0} already exists on this object");
				return;
			}

			component->SetCheezyObject(Get());
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
		void SetShape(const Ref<T>& shape)
		{
			static_assert(std::is_base_of<Shape2D, T>::value, "The passed shape isn't a shape2D");
			m_Shape = shape;
		}

		Ref<CheezyObject> Get() { return shared_from_this(); }

		const char* GetTag() { return m_Tag; }
		void SetTag(const char* tag) { m_Tag = tag; }

		void OnCollision(const Collision2D& collision);
		void OnCollisionEnter(const Collision2D& collision);
		void OnCollisionExit(const Collision2D& collision);

	private:
		std::vector<Ref<CheezyComponent>> m_Components;
		Ref<Shape2D> m_Shape;
		const char* m_Tag;
	};
}