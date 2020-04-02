#pragma once

#include "Hazel/Input.h"
#include "Hazel/Core.h"
#include "Hazel/Window.h"

namespace Hazel
{
	class WindowsInput : public Input
	{
	public:

	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePosImpl() override;

	};
}