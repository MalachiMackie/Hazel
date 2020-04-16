#pragma once

#include "Hazel/Core/Input.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Core/Window.h"
#include "Hazel/Core/MouseCodes.h"
#include "Hazel/Core/KeyCodes.h"

namespace Hazel
{
	class WindowsInput : public Input
	{
	public:

	protected:
		virtual bool IsKeyPressedImpl(KeyCode keycode) override;
		virtual bool IsMouseButtonPressedImpl(MouseCode button) override;
		virtual std::pair<float, float> GetMousePosImpl() override;

	};
}