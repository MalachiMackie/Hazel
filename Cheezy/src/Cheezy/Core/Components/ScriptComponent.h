#pragma once

#include "Cheezy/Core/Timestep.h"
#include "Cheezy/Core/Components/CheezyComponent.h"
#include "Cheezy/Core/Components/BoxCollider2DComponent.h"
#include "Cheezy/Core/Collision2D.h"
#include "Cheezy/Core/CheezyObject.h"
#include "Cheezy/Core/KeyCodes.h"

#include <string>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace Cheezy
{
	class ScriptComponent : public CheezyComponent
	{
	public:
		ScriptComponent(const char* fileName);
		~ScriptComponent() override;

		virtual void Init() override;

		virtual bool IsUniqueComponent() const { return false; }
		virtual std::string GetComponentName() const { return "Script"; }

		virtual void OnCollision(Collision2D collision) override;
		virtual void OnCollisionEnter(Collision2D collision) override;
		virtual void OnCollisionExit(Collision2D collision) override;

		virtual void OnFixedUpdate() override;
		virtual void OnUpdate(Timestep ts) override;

	private:
		lua_State* m_LuaState;
		const char* m_FilePath;
		bool m_IsInitiailzed = false;

		void InitInputCodes();
	};
}