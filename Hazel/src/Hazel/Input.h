#pragma once

#include "Hazel/Core.h"
#include "Hazel/Window.h"

namespace Hazel
{
	class HAZEL_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static float GetMouseX() { return GetMousePos().second; }
		inline static float GetMouseY() { return GetMousePos().first; }
		inline static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); }


	protected:

		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int keycode) = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;

	private:
		static Input* s_Instance;
	};
}