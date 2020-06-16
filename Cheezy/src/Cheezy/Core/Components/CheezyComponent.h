#pragma once

#include "Cheezy/Core/CheezyObject.h"
#include "Cheezy/Core/Timestep.h"
#include "Cheezy/Core/Collision2D.h"
#include "Cheezy/Events/Event.h"

#include <string>

namespace Cheezy
{
	class CheezyComponent
	{
	public:
		CheezyComponent() = default;
		virtual ~CheezyComponent() = default;

		virtual void Init() {};

		virtual bool IsUniqueComponent() const = 0;
		virtual std::string GetComponentName() const = 0;

		virtual void OnStart() {}
		/*
			Runs 50 times per second.
			Will run multiple times per rendered frame if it has to
		*/
		virtual void OnFixedUpdate() {}
		virtual void OnUpdate(Timestep ts) {}

		virtual void OnCollision(const Collision2D& collision) {}
		virtual void OnCollisionEnter(const Collision2D& collision) {}
		virtual void OnCollisionExit(const Collision2D& collision) {}

		virtual void OnTrigger(const Collision2D& collision) {}
		virtual void OnTriggerEnter(const Collision2D& collision) {}
		virtual void OnTriggerExit(const Collision2D& collision) {}

		virtual void OnEvent(Event& e) {}

		virtual void OnMouseEnter() {}
		virtual void OnMouseHover() {}
		virtual void OnMouseExit() {}

		void SetCheezyObject(const Ref<CheezyObject>& cheezyObject) { m_CheezyObject = cheezyObject; }
		Ref<CheezyObject>& GetCheezyObject() { return m_CheezyObject; }


	protected:
		Ref<CheezyObject> m_CheezyObject;
	};
}