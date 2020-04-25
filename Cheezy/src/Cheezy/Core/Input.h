#pragma once

#include "Cheezy/Core/Window.h"
#include "Cheezy/Core/KeyCodes.h"
#include "Cheezy/Core/MouseCodes.h"

namespace Cheezy
{
	class Input
	{
	protected:
		Input() = default;

	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool IsKeyPressed(KeyCode keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); }
		inline static float GetMouseX() { return GetMousePos().second; }
		inline static float GetMouseY() { return GetMousePos().first; }

		static Scope<Input> Create();

	protected:
		virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseCode keycode) = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;

	private:
		static Scope<Input> s_Instance;
	};
}