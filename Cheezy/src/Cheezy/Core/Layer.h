#pragma once

#include "Cheezy/Events/Event.h"
#include "Cheezy/Core/Timestep.h"

namespace Cheezy
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		/*
			Runs 50 times per second.
			Will run multiple times per rendered frame if it has to
		*/
		virtual void OnFixedUpdate() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}

