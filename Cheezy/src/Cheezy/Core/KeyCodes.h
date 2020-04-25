#pragma once
namespace Cheezy
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

#define CZ_KEY_SPACE           ::Cheezy::Key::Space
#define CZ_KEY_APOSTROPHE      ::Cheezy::Key::Apostrophe    /* ' */
#define CZ_KEY_COMMA           ::Cheezy::Key::Comma         /* , */
#define CZ_KEY_MINUS           ::Cheezy::Key::Minus         /* - */
#define CZ_KEY_PERIOD          ::Cheezy::Key::Period        /* . */
#define CZ_KEY_SLASH           ::Cheezy::Key::Slash         /* / */
#define CZ_KEY_0               ::Cheezy::Key::D0
#define CZ_KEY_1               ::Cheezy::Key::D1
#define CZ_KEY_2               ::Cheezy::Key::D2
#define CZ_KEY_3               ::Cheezy::Key::D3
#define CZ_KEY_4               ::Cheezy::Key::D4
#define CZ_KEY_5               ::Cheezy::Key::D5
#define CZ_KEY_6               ::Cheezy::Key::D6
#define CZ_KEY_7               ::Cheezy::Key::D7
#define CZ_KEY_8               ::Cheezy::Key::D8
#define CZ_KEY_9               ::Cheezy::Key::D9
#define CZ_KEY_SEMICOLON       ::Cheezy::Key::Semicolon     /* ; */
#define CZ_KEY_EQUAL           ::Cheezy::Key::Equal         /* = */
#define CZ_KEY_A               ::Cheezy::Key::A
#define CZ_KEY_B               ::Cheezy::Key::B
#define CZ_KEY_C               ::Cheezy::Key::C
#define CZ_KEY_D               ::Cheezy::Key::D
#define CZ_KEY_E               ::Cheezy::Key::E
#define CZ_KEY_F               ::Cheezy::Key::F
#define CZ_KEY_G               ::Cheezy::Key::G
#define CZ_KEY_H               ::Cheezy::Key::H
#define CZ_KEY_I               ::Cheezy::Key::I
#define CZ_KEY_J               ::Cheezy::Key::J
#define CZ_KEY_K               ::Cheezy::Key::K
#define CZ_KEY_L               ::Cheezy::Key::L
#define CZ_KEY_M               ::Cheezy::Key::M
#define CZ_KEY_N               ::Cheezy::Key::N
#define CZ_KEY_O               ::Cheezy::Key::O
#define CZ_KEY_P               ::Cheezy::Key::P
#define CZ_KEY_Q               ::Cheezy::Key::Q
#define CZ_KEY_R               ::Cheezy::Key::R
#define CZ_KEY_S               ::Cheezy::Key::S
#define CZ_KEY_T               ::Cheezy::Key::T
#define CZ_KEY_U               ::Cheezy::Key::U
#define CZ_KEY_V               ::Cheezy::Key::V
#define CZ_KEY_W               ::Cheezy::Key::W
#define CZ_KEY_X               ::Cheezy::Key::X
#define CZ_KEY_Y               ::Cheezy::Key::Y
#define CZ_KEY_Z               ::Cheezy::Key::Z
#define CZ_KEY_LEFT_BRACKET    ::Cheezy::Key::LeftBracket   /* [ */
#define CZ_KEY_BACKSLASH       ::Cheezy::Key::Backslash     /* \ */
#define CZ_KEY_RIGHT_BRACKET   ::Cheezy::Key::RightBracket  /* ] */
#define CZ_KEY_GRAVE_ACCENT    ::Cheezy::Key::GraveAccent   /* ` */
#define CZ_KEY_WORLD_1         ::Cheezy::Key::World1        /* non-US #1 */
#define CZ_KEY_WORLD_2         ::Cheezy::Key::World2        /* non-US #2 */

#define CZ_KEY_ESCAPE          ::Cheezy::Key::Escape
#define CZ_KEY_ENTER           ::Cheezy::Key::Enter
#define CZ_KEY_TAB             ::Cheezy::Key::Tab
#define CZ_KEY_BACKSPACE       ::Cheezy::Key::Backspace
#define CZ_KEY_INSERT          ::Cheezy::Key::Insert
#define CZ_KEY_DELETE          ::Cheezy::Key::Delete
#define CZ_KEY_RIGHT           ::Cheezy::Key::Right
#define CZ_KEY_LEFT            ::Cheezy::Key::Left
#define CZ_KEY_DOWN            ::Cheezy::Key::Down
#define CZ_KEY_UP              ::Cheezy::Key::Up
#define CZ_KEY_PAGE_UP         ::Cheezy::Key::PageUp
#define CZ_KEY_PAGE_DOWN       ::Cheezy::Key::PageDown
#define CZ_KEY_HOME            ::Cheezy::Key::Home
#define CZ_KEY_END             ::Cheezy::Key::End
#define CZ_KEY_CAPS_LOCK       ::Cheezy::Key::CapsLock
#define CZ_KEY_SCROLL_LOCK     ::Cheezy::Key::ScrollLock
#define CZ_KEY_NUM_LOCK        ::Cheezy::Key::NumLock
#define CZ_KEY_PRINT_SCREEN    ::Cheezy::Key::PrintScreen
#define CZ_KEY_PAUSE           ::Cheezy::Key::Pause
#define CZ_KEY_F1              ::Cheezy::Key::F1
#define CZ_KEY_F2              ::Cheezy::Key::F2
#define CZ_KEY_F3              ::Cheezy::Key::F3
#define CZ_KEY_F4              ::Cheezy::Key::F4
#define CZ_KEY_F5              ::Cheezy::Key::F5
#define CZ_KEY_F6              ::Cheezy::Key::F6
#define CZ_KEY_F7              ::Cheezy::Key::F7
#define CZ_KEY_F8              ::Cheezy::Key::F8
#define CZ_KEY_F9              ::Cheezy::Key::F9
#define CZ_KEY_F10             ::Cheezy::Key::F10
#define CZ_KEY_F11             ::Cheezy::Key::F11
#define CZ_KEY_F12             ::Cheezy::Key::F12
#define CZ_KEY_F13             ::Cheezy::Key::F13
#define CZ_KEY_F14             ::Cheezy::Key::F14
#define CZ_KEY_F15             ::Cheezy::Key::F15
#define CZ_KEY_F16             ::Cheezy::Key::F16
#define CZ_KEY_F17             ::Cheezy::Key::F17
#define CZ_KEY_F18             ::Cheezy::Key::F18
#define CZ_KEY_F19             ::Cheezy::Key::F19
#define CZ_KEY_F20             ::Cheezy::Key::F20
#define CZ_KEY_F21             ::Cheezy::Key::F21
#define CZ_KEY_F22             ::Cheezy::Key::F22
#define CZ_KEY_F23             ::Cheezy::Key::F23
#define CZ_KEY_F24             ::Cheezy::Key::F24
#define CZ_KEY_F25             ::Cheezy::Key::F25

/* Keypad */
#define CZ_KEY_KP_0            ::Cheezy::Key::KP0
#define CZ_KEY_KP_1            ::Cheezy::Key::KP1
#define CZ_KEY_KP_2            ::Cheezy::Key::KP2
#define CZ_KEY_KP_3            ::Cheezy::Key::KP3
#define CZ_KEY_KP_4            ::Cheezy::Key::KP4
#define CZ_KEY_KP_5            ::Cheezy::Key::KP5
#define CZ_KEY_KP_6            ::Cheezy::Key::KP6
#define CZ_KEY_KP_7            ::Cheezy::Key::KP7
#define CZ_KEY_KP_8            ::Cheezy::Key::KP8
#define CZ_KEY_KP_9            ::Cheezy::Key::KP9
#define CZ_KEY_KP_DECIMAL      ::Cheezy::Key::KPDecimal
#define CZ_KEY_KP_DIVIDE       ::Cheezy::Key::KPDivide
#define CZ_KEY_KP_MULTIPLY     ::Cheezy::Key::KPMultiply
#define CZ_KEY_KP_SUBTRACT     ::Cheezy::Key::KPSubtract
#define CZ_KEY_KP_ADD          ::Cheezy::Key::KPAdd
#define CZ_KEY_KP_ENTER        ::Cheezy::Key::KPEnter
#define CZ_KEY_KP_EQUAL        ::Cheezy::Key::KPEqual

#define CZ_KEY_LEFT_SHIFT      ::Cheezy::Key::LeftShift
#define CZ_KEY_LEFT_CONTROL    ::Cheezy::Key::LeftControl
#define CZ_KEY_LEFT_ALT        ::Cheezy::Key::LeftAlt
#define CZ_KEY_LEFT_SUPER      ::Cheezy::Key::LeftSuper
#define CZ_KEY_RIGHT_SHIFT     ::Cheezy::Key::RightShift
#define CZ_KEY_RIGHT_CONTROL   ::Cheezy::Key::RightControl
#define CZ_KEY_RIGHT_ALT       ::Cheezy::Key::RightAlt
#define CZ_KEY_RIGHT_SUPER     ::Cheezy::Key::RightSuper
#define CZ_KEY_MENU            ::Cheezy::Key::Menu
