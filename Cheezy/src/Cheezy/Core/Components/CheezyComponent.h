#pragma once

#include "Cheezy/Core/Timestep.h"

#include <string>

namespace Cheezy
{
	class CheezyObject;

	class CheezyComponent
	{
	public:
		CheezyComponent() = default;
		virtual ~CheezyComponent() = default;

		virtual void Init() {};

		virtual bool IsUniqueComponent() const = 0;
		virtual std::string GetComponentName() const = 0;

		virtual void OnUpdate(Timestep ts) {}


	protected:
		Ref<CheezyObject> m_CheezyObject;

		friend class CheezyObject;
	};
}