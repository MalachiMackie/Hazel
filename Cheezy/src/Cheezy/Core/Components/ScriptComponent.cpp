#include "hzpch.h"
#include "ScriptComponent.h"

#include "Cheezy/Core/Input.h"
#include "Cheezy/Core/KeyCodes.h"
#include "Cheezy/Core/MouseCodes.h"
#include "Cheezy/Core/Transform2D.h"

#include "Cheezy/Core/Components/Transform2DComponent.h"
#include "Cheezy/Core/Components/RigidBody2DComponent.h"

#include <LuaBridge/LuaBridge.h>
#include <functional>

namespace Cheezy
{
	static bool IsKeyPressed(int keycode)
	{
		return Input::IsKeyPressed((KeyCode)keycode);
	}

	template<typename T>
	static T* GetCheezyObjectComponent(const CheezyObject* obj)
	{
		return obj->GetComponent<T>().get();
	}

	static float GetVec2Magnitude(const glm::vec2* vec)
	{
		return glm::length(*vec);
	}

	ScriptComponent::ScriptComponent(const char* filePath)
		: m_LuaState(luaL_newstate()), m_FilePath(filePath)
	{
	}

	ScriptComponent::~ScriptComponent()
	{
		lua_close(m_LuaState);
	}

	void ScriptComponent::OnCollision(Collision2D collision)
	{
		if (!m_IsInitiailzed)
		{
			return;
		}

		static luabridge::LuaRef onCollisionDetectedFunc{ luabridge::getGlobal(m_LuaState, "OnCollision") };
		if (onCollisionDetectedFunc.isFunction())
		{
			try
			{
				auto _ = onCollisionDetectedFunc(collision);
			}
			catch (luabridge::LuaException & ex)
			{
				CZ_ERROR("Lua Error Occurred: {0}", ex.what());
			}
		}
	}

	void ScriptComponent::OnCollisionEnter(Collision2D collision)
	{
		if (!m_IsInitiailzed)
		{
			return;
		}

		static luabridge::LuaRef onCollisionDetectedFunc{ luabridge::getGlobal(m_LuaState, "OnCollisionEnter") };
		if (onCollisionDetectedFunc.isFunction())
		{
			try
			{
				auto _ = onCollisionDetectedFunc(collision);
			}
			catch (luabridge::LuaException & ex)
			{
				CZ_ERROR("Lua Error Occurred: {0}", ex.what());
			}
		}
	}

	void ScriptComponent::OnCollisionExit(Collision2D collision)
	{
		if (!m_IsInitiailzed)
		{
			return;
		}

		static luabridge::LuaRef onCollisionDetectedFunc{ luabridge::getGlobal(m_LuaState, "OnCollisionExit") };
		if (onCollisionDetectedFunc.isFunction())
		{
			try
			{
				auto _ = onCollisionDetectedFunc(collision);
			}
			catch (luabridge::LuaException & ex)
			{
				CZ_ERROR("Lua Error Occurred: {0}", ex.what());
			}
		}
	}

	void ScriptComponent::OnFixedUpdate()
	{
		if (!m_IsInitiailzed)
		{
			return;
		}

		static luabridge::LuaRef onFixedUpdateFunc{ luabridge::getGlobal(m_LuaState, "OnFixedUpdate") };
		if (onFixedUpdateFunc.isFunction())
		{
			try
			{
				auto _ = onFixedUpdateFunc();
			}
			catch (luabridge::LuaException & ex)
			{
				CZ_ERROR("Lua Error Occurred: {0}", ex.what());
			}
		}
	}

	void ScriptComponent::OnUpdate(Timestep ts)
	{
		if (!m_IsInitiailzed)
		{
			return;
		}

		static luabridge::LuaRef onUpdateFunc{ luabridge::getGlobal(m_LuaState, "OnUpdate") };
		if (onUpdateFunc.isFunction())
		{
			try
			{
				auto _ = onUpdateFunc(ts.GetSeconds());
			}
			catch (luabridge::LuaException& ex)
			{
				CZ_ERROR("Lua Error Occurred: {0}", ex.what());
			}
		}
	}

	void ScriptComponent::Init()
	{
		CZ_PROFILE_FUNCTION();
		luaL_openlibs(m_LuaState);

		auto luaNameSpace =
			luabridge::getGlobalNamespace(m_LuaState)
			.beginClass<CheezyObject>("CheezyObject")
				.addFunction("GetTransform", GetCheezyObjectComponent<Transform2DComponent>)
				.addFunction("GetRigidBody", GetCheezyObjectComponent<RigidBodyComponent>)
			.endClass()
			.beginClass<CheezyComponent>("CheezyComponent").endClass()
				.deriveClass<Transform2DComponent, CheezyComponent>("Transform")
					.addProperty("Transform", &Transform2DComponent::GetTransform)
				.endClass()
				.deriveClass<RigidBodyComponent, CheezyComponent>("RigidBody")
					.addFunction("GetVelocity", &RigidBodyComponent::GetVelocity)
					.addFunction("AddForce", &RigidBodyComponent::AddForce)
				.endClass()
			.beginClass<glm::vec3>("Vec3")
				.addConstructor<void(*) (float, float, float)>()
				.addProperty("x", &glm::vec3::x, true)
				.addProperty("y", &glm::vec3::y, true)
				.addProperty("z", &glm::vec3::z, true)
			.endClass()
			.beginClass<glm::vec2>("Vec2")
				.addConstructor<void(*) (float, float)>()
				.addProperty("x", &glm::vec2::x, true)
				.addProperty("y", &glm::vec2::y, true)
				.addFunction("GetMagnitude", &GetVec2Magnitude)
			.endClass()
			.beginClass<Collision2D>("Collision2D")
			.endClass()
			.beginClass<Transform2D>("Transform2D")
				.addConstructor<void(*) (glm::vec3, glm::vec2, float)>()
				.addProperty("Position", &Transform2D::Position, true)
				.addProperty("Scale", &Transform2D::Scale, true)
				.addProperty("Rotation", &Transform2D::Rotation, true)
			.endClass()
			.addFunction("IsKeyDown", IsKeyPressed);

		luabridge::setGlobal(m_LuaState, GetCheezyObject().get(), "MyCheezyObject");

		InitInputCodes();

		if (luaL_dofile(m_LuaState, m_FilePath) != LUA_OK)
		{
			const char* error = luaL_tolstring(m_LuaState, -1, nullptr);
			CZ_ERROR("Lua script could not compile({0}) :{1}", m_FilePath, error);
			return;
		}
		m_IsInitiailzed = true;

		luabridge::setGlobal(m_LuaState, CZ_TRANSFORM_2D_COMPONENT, "CZ_TRANSFORM_2D_COMPONENT");
	}

	void ScriptComponent::InitInputCodes()
	{
#pragma region KeyCodes
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_SPACE ,"CZ_KEY_SPACE");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_APOSTROPHE, "CZ_KEY_APOSTROPHE");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_COMMA, "CZ_KEY_COMMA");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_MINUS, "CZ_KEY_MINUS");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_PERIOD, "CZ_KEY_PERIOD");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_SLASH, "CZ_KEY_SLASH");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_0, "CZ_KEY_0");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_1, "CZ_KEY_1");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_2, "CZ_KEY_2");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_3, "CZ_KEY_3");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_4, "CZ_KEY_4");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_5, "CZ_KEY_5");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_6, "CZ_KEY_6");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_7, "CZ_KEY_7");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_8, "CZ_KEY_8");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_9, "CZ_KEY_9");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_SEMICOLON, "CZ_KEY_SEMICOLON");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_EQUAL, "CZ_KEY_EQUAL");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_A, "CZ_KEY_A");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_B, "CZ_KEY_B");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_C, "CZ_KEY_C");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_D, "CZ_KEY_D");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_E, "CZ_KEY_E");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F, "CZ_KEY_F");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_G, "CZ_KEY_G");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_H, "CZ_KEY_H");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_I, "CZ_KEY_I");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_J, "CZ_KEY_J");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_K, "CZ_KEY_K");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_L, "CZ_KEY_L");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_M, "CZ_KEY_M");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_N, "CZ_KEY_N");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_O, "CZ_KEY_O");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_P, "CZ_KEY_P");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_Q, "CZ_KEY_Q");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_R, "CZ_KEY_R");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_S, "CZ_KEY_S");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_T, "CZ_KEY_T");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_U, "CZ_KEY_U");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_V, "CZ_KEY_V");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_W, "CZ_KEY_W");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_X, "CZ_KEY_X");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_Y, "CZ_KEY_Y");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_Z, "CZ_KEY_Z");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_LEFT_BRACKET, "CZ_KEY_LEFT_BRACKET");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_BACKSLASH, "CZ_KEY_BACKSLASH");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_RIGHT_BRACKET, "CZ_KEY_RIGHT_BRACKET");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_GRAVE_ACCENT, "CZ_KEY_GRAVE_ACCENT");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_WORLD_1, "CZ_KEY_WORLD_1");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_WORLD_2, "CZ_KEY_WORLD_2");
							 
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_ESCAPE, "CZ_KEY_ESCAPE");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_ENTER, "CZ_KEY_ENTER");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_TAB, "CZ_KEY_TAB");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_BACKSPACE, "CZ_KEY_BACKSPACE");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_INSERT, "CZ_KEY_INSERT");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_DELETE, "CZ_KEY_DELETE");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_RIGHT, "CZ_KEY_RIGHT");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_LEFT, "CZ_KEY_LEFT");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_DOWN, "CZ_KEY_DOWN");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_UP, "CZ_KEY_UP");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_PAGE_UP, "CZ_KEY_PAGE_UP");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_PAGE_DOWN, "CZ_KEY_PAGE_DOWN");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_HOME, "CZ_KEY_HOME");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_END, "CZ_KEY_END");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_CAPS_LOCK, "CZ_KEY_CAPS_LOCK");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_SCROLL_LOCK, "CZ_KEY_SCROLL_LOCK");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_NUM_LOCK, "CZ_KEY_NUM_LOCK");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_PRINT_SCREEN, "CZ_KEY_PRINT_SCREEN");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_PAUSE, "CZ_KEY_PAUSE");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F1, "CZ_KEY_F1");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F2, "CZ_KEY_F2");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F3, "CZ_KEY_F3");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F4, "CZ_KEY_F4");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F5, "CZ_KEY_F5");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F6, "CZ_KEY_F6");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F7, "CZ_KEY_F7");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F8, "CZ_KEY_F8");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F9, "CZ_KEY_F9");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F10, "CZ_KEY_F10");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F11, "CZ_KEY_F11");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F12, "CZ_KEY_F12");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F13, "CZ_KEY_F13");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F14, "CZ_KEY_F14");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F15, "CZ_KEY_F15");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F16, "CZ_KEY_F16");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F17, "CZ_KEY_F17");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F18, "CZ_KEY_F18");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F19, "CZ_KEY_F19");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F20, "CZ_KEY_F20");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F21, "CZ_KEY_F21");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F22, "CZ_KEY_F22");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F23, "CZ_KEY_F23");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F24, "CZ_KEY_F24");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_F25, "CZ_KEY_F25");
							 
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_0, "CZ_KEY_KP_0");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_1, "CZ_KEY_KP_1");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_2, "CZ_KEY_KP_2");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_3, "CZ_KEY_KP_3");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_4, "CZ_KEY_KP_4");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_5, "CZ_KEY_KP_5");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_6, "CZ_KEY_KP_6");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_7, "CZ_KEY_KP_7");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_8, "CZ_KEY_KP_8");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_9, "CZ_KEY_KP_9");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_DECIMAL, "CZ_KEY_KP_DECIMAL");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_DIVIDE, "CZ_KEY_KP_DIVIDE");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_MULTIPLY, "CZ_KEY_KP_MULTIPLY");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_SUBTRACT, "CZ_KEY_KP_SUBTRACT");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_ADD, "CZ_KEY_KP_ADD");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_ENTER, "CZ_KEY_KP_ENTER");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_KP_EQUAL, "CZ_KEY_KP_EQUAL");
							 
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_LEFT_SHIFT, "CZ_KEY_LEFT_SHIFT");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_LEFT_CONTROL, "CZ_KEY_LEFT_CONTROL");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_LEFT_ALT, "CZ_KEY_LEFT_ALT");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_LEFT_SUPER, "CZ_KEY_LEFT_SUPER");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_RIGHT_SHIFT, "CZ_KEY_RIGHT_SHIFT");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_RIGHT_CONTROL, "CZ_KEY_RIGHT_CONTROL");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_RIGHT_ALT, "CZ_KEY_RIGHT_ALT");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_RIGHT_SUPER, "CZ_KEY_RIGHT_SUPER");
		luabridge::setGlobal(m_LuaState, (int)CZ_KEY_MENU, "CZ_KEY_MENU");
#pragma endregion			
							
#pragma region MouseCodes	
		luabridge::setGlobal(m_LuaState, (int)CZ_MOUSE_BUTTON_0, "CZ_MOUSE_BUTTON_0");
		luabridge::setGlobal(m_LuaState, (int)CZ_MOUSE_BUTTON_1, "CZ_MOUSE_BUTTON_1");
		luabridge::setGlobal(m_LuaState, (int)CZ_MOUSE_BUTTON_2, "CZ_MOUSE_BUTTON_2");
		luabridge::setGlobal(m_LuaState, (int)CZ_MOUSE_BUTTON_3, "CZ_MOUSE_BUTTON_3");
		luabridge::setGlobal(m_LuaState, (int)CZ_MOUSE_BUTTON_4, "CZ_MOUSE_BUTTON_4");
		luabridge::setGlobal(m_LuaState, (int)CZ_MOUSE_BUTTON_5, "CZ_MOUSE_BUTTON_5");
		luabridge::setGlobal(m_LuaState, (int)CZ_MOUSE_BUTTON_6, "CZ_MOUSE_BUTTON_6");
		luabridge::setGlobal(m_LuaState, (int)CZ_MOUSE_BUTTON_7, "CZ_MOUSE_BUTTON_7");
		luabridge::setGlobal(m_LuaState, (int)CZ_MOUSE_BUTTON_LAST, "CZ_MOUSE_BUTTON_LAST");
		luabridge::setGlobal(m_LuaState, (int)CZ_MOUSE_BUTTON_LEFT, "CZ_MOUSE_BUTTON_LEFT");
		luabridge::setGlobal(m_LuaState, (int)CZ_MOUSE_BUTTON_RIGHT, "CZ_MOUSE_BUTTON_RIGHT");
		luabridge::setGlobal(m_LuaState, (int)CZ_MOUSE_BUTTON_MIDDLE, "CZ_MOUSE_BUTTON_MIDDLE");
#pragma endregion
	}
}