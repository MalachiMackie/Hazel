#pragma once

#include "Hazel/Core/Input.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Core/Window.h"

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