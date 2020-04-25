#pragma once

#include <iostream>

namespace Cheezy
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define CZ_MOUSE_BUTTON_0      ::Cheezy::Mouse::Button0
#define CZ_MOUSE_BUTTON_1      ::Cheezy::Mouse::Button1
#define CZ_MOUSE_BUTTON_2      ::Cheezy::Mouse::Button2
#define CZ_MOUSE_BUTTON_3      ::Cheezy::Mouse::Button3
#define CZ_MOUSE_BUTTON_4      ::Cheezy::Mouse::Button4
#define CZ_MOUSE_BUTTON_5      ::Cheezy::Mouse::Button5
#define CZ_MOUSE_BUTTON_6      ::Cheezy::Mouse::Button6
#define CZ_MOUSE_BUTTON_7      ::Cheezy::Mouse::Button7
#define CZ_MOUSE_BUTTON_LAST   ::Cheezy::Mouse::ButtonLast
#define CZ_MOUSE_BUTTON_LEFT   ::Cheezy::Mouse::ButtonLeft
#define CZ_MOUSE_BUTTON_RIGHT  ::Cheezy::Mouse::ButtonRight
#define CZ_MOUSE_BUTTON_MIDDLE ::Cheezy::Mouse::ButtonMiddle
