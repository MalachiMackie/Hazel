#pragma once

#include "Cheezy/Core/Timestep.h"
#include "Cheezy/Core/Components/CheezyComponent.h"
#include "Cheezy/Core/Components/BoxCollider2DComponent.h"
#include "Cheezy/Core/Collision2D.h"
#include "Cheezy/Core/CheezyObject.h"
#include "Cheezy/Core/KeyCodes.h"

#include <string>

namespace Cheezy
{
	class ScriptComponent : public CheezyComponent
	{
	public:
		virtual bool IsUniqueComponent() const override { return false; }
		virtual std::string GetComponentName() const override { return "Script"; }
	};
}