#pragma once

#include "Cheezy/Core/Input.h"
#include "Cheezy/Core/Core.h"
#include "Cheezy/Core/Window.h"
#include "Cheezy/Core/MouseCodes.h"
#include "Cheezy/Core/KeyCodes.h"

namespace Cheezy
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