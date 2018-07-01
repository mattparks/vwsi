#ifndef VULKAN_WSI_HPP
#define VULKAN_WSI_HPP 1

/*
** Copyright 2018 Matthew Albrecht
**
** Permission is hereby granted, free of charge, to any person obtaining a copy of this
** software and associated documentation files (the "Software"), to deal in the Software
** without restriction, including without limitation the rights to use, copy, modify, merge,
** publish, distribute, sublicense, and/or sell copies of the Software, and to permit
** persons to whom the Software is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all copies or
** substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
** BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
** DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <vulkan/vulkan.hpp>
#include <VWSI/vulkan_wsi.h>

#if !defined(VULKAN_WSI_HPP_NAMESPACE)
#define VULKAN_WSI_HPP_NAMESPACE wsi
#endif

namespace VULKAN_HPP_NAMESPACE
{
	enum Key
	{
		WSI_KEY_UNKNOWN = -1,
		WSI_KEY_SPACE = 32,
		WSI_KEY_APOSTROPHE = 39,
		WSI_KEY_COMMA = 44,
		WSI_KEY_MINUS = 45,
		WSI_KEY_PERIOD = 46,
		WSI_KEY_SLASH = 47,
		WSI_KEY_0 = 48,
		WSI_KEY_1 = 49,
		WSI_KEY_2 = 50,
		WSI_KEY_3 = 51,
		WSI_KEY_4 = 52,
		WSI_KEY_5 = 53,
		WSI_KEY_6 = 54,
		WSI_KEY_7 = 55,
		WSI_KEY_8 = 56,
		WSI_KEY_9 = 57,
		WSI_KEY_SEMICOLON = 59,
		WSI_KEY_EQUAL = 61,
		WSI_KEY_A = 65,
		WSI_KEY_B = 66,
		WSI_KEY_C = 67,
		WSI_KEY_D = 68,
		WSI_KEY_E = 69,
		WSI_KEY_F = 70,
		WSI_KEY_G = 71,
		WSI_KEY_H = 72,
		WSI_KEY_I = 73,
		WSI_KEY_J = 74,
		WSI_KEY_K = 75,
		WSI_KEY_L = 76,
		WSI_KEY_M = 77,
		WSI_KEY_N = 78,
		WSI_KEY_O = 79,
		WSI_KEY_P = 80,
		WSI_KEY_Q = 81,
		WSI_KEY_R = 82,
		WSI_KEY_S = 83,
		WSI_KEY_T = 84,
		WSI_KEY_U = 85,
		WSI_KEY_V = 86,
		WSI_KEY_W = 87,
		WSI_KEY_X = 88,
		WSI_KEY_Y = 89,
		WSI_KEY_Z = 90,
		WSI_KEY_LEFT_BRACKET = 91,
		WSI_KEY_BACKSLASH = 92,
		WSI_KEY_RIGHT_BRACKET = 93,
		WSI_KEY_GRAVE_ACCENT = 96,
		WSI_KEY_WORLD_1 = 161,
		WSI_KEY_WORLD_2 = 162,
		WSI_KEY_ESCAPE = 256,
		WSI_KEY_ENTER = 257,
		WSI_KEY_TAB = 258,
		WSI_KEY_BACKSPACE = 259,
		WSI_KEY_INSERT = 260,
		WSI_KEY_DELETE = 261,
		WSI_KEY_RIGHT = 262,
		WSI_KEY_LEFT = 263,
		WSI_KEY_DOWN = 264,
		WSI_KEY_UP = 265,
		WSI_KEY_PAGE_UP = 266,
		WSI_KEY_PAGE_DOWN = 267,
		WSI_KEY_HOME = 268,
		WSI_KEY_END = 269,
		WSI_KEY_CAPS_LOCK = 280,
		WSI_KEY_SCROLL_LOCK = 281,
		WSI_KEY_NUM_LOCK = 282,
		WSI_KEY_PRINT_SCREEN = 283,
		WSI_KEY_PAUSE = 284,
		WSI_KEY_F1 = 290,
		WSI_KEY_F2 = 291,
		WSI_KEY_F3 = 292,
		WSI_KEY_F4 = 293,
		WSI_KEY_F5 = 294,
		WSI_KEY_F6 = 295,
		WSI_KEY_F7 = 296,
		WSI_KEY_F8 = 297,
		WSI_KEY_F9 = 298,
		WSI_KEY_F10 = 299,
		WSI_KEY_F11 = 300,
		WSI_KEY_F12 = 301,
		WSI_KEY_F13 = 302,
		WSI_KEY_F14 = 303,
		WSI_KEY_F15 = 304,
		WSI_KEY_F16 = 305,
		WSI_KEY_F17 = 306,
		WSI_KEY_F18 = 307,
		WSI_KEY_F19 = 308,
		WSI_KEY_F20 = 309,
		WSI_KEY_F21 = 310,
		WSI_KEY_F22 = 311,
		WSI_KEY_F23 = 312,
		WSI_KEY_F24 = 313,
		WSI_KEY_F25 = 314,
		WSI_KEY_KP_0 = 320,
		WSI_KEY_KP_1 = 321,
		WSI_KEY_KP_2 = 322,
		WSI_KEY_KP_3 = 323,
		WSI_KEY_KP_4 = 324,
		WSI_KEY_KP_5 = 325,
		WSI_KEY_KP_6 = 326,
		WSI_KEY_KP_7 = 327,
		WSI_KEY_KP_8 = 328,
		WSI_KEY_KP_9 = 329,
		WSI_KEY_KP_DECIMAL = 330,
		WSI_KEY_KP_DIVIDE = 331,
		WSI_KEY_KP_MULTIPLY = 332,
		WSI_KEY_KP_SUBTRACT = 333,
		WSI_KEY_KP_ADD = 334,
		WSI_KEY_KP_ENTER = 335,
		WSI_KEY_KP_EQUAL = 336,
		WSI_KEY_LEFT_SHIFT = 340,
		WSI_KEY_LEFT_CONTROL = 341,
		WSI_KEY_LEFT_ALT = 342,
		WSI_KEY_LEFT_SUPER = 343,
		WSI_KEY_RIGHT_SHIFT = 344,
		WSI_KEY_RIGHT_CONTROL = 345,
		WSI_KEY_RIGHT_ALT = 346,
		WSI_KEY_RIGHT_SUPER = 347,
		WSI_KEY_MENU = 348,
		WSI_KEY_LAST = WSI_KEY_MENU
	};

	enum MouseButton
	{
		eButton1 = WSI_MOUSE_BUTTON_1,
		eButton2 = WSI_MOUSE_BUTTON_2,
		eButton3 = WSI_MOUSE_BUTTON_3,
		eButton4 = WSI_MOUSE_BUTTON_4,
		eButton5 = WSI_MOUSE_BUTTON_5,
		eButton6 = WSI_MOUSE_BUTTON_6,
		eButton7 = WSI_MOUSE_BUTTON_7,
		eButton8 = WSI_MOUSE_BUTTON_8,
		eButtonLast = WSI_MOUSE_BUTTON_LAST,
		eButtonLeft = WSI_MOUSE_BUTTON_LEFT,
		eButtonRight = WSI_MOUSE_BUTTON_RIGHT,
		eButtonMiddle = WSI_MOUSE_BUTTON_MIDDLE
	};

	enum Joystick
	{
		WSI_JOYSTICK_1 = 0,
		WSI_JOYSTICK_2 = 1,
		WSI_JOYSTICK_3 = 2,
		WSI_JOYSTICK_4 = 3,
		WSI_JOYSTICK_5 = 4,
		WSI_JOYSTICK_6 = 5,
		WSI_JOYSTICK_7 = 6,
		WSI_JOYSTICK_8 = 7,
		WSI_JOYSTICK_9 = 8,
		WSI_JOYSTICK_10 = 9,
		WSI_JOYSTICK_11 = 10,
		WSI_JOYSTICK_12 = 11,
		WSI_JOYSTICK_13 = 12,
		WSI_JOYSTICK_14 = 13,
		WSI_JOYSTICK_15 = 14,
		WSI_JOYSTICK_16 = 15,
		WSI_JOYSTICK_LAST = WSI_JOYSTICK_16
	};

	enum Action
	{
		WSI_ACTION_PRESS = 0x0001,
		WSI_ACTION_RELEASE = 0x0002,
		WSI_ACTION_REPEAT = 0x0003,
		WSI_ACTION_LAST = WSI_ACTION_REPEAT
	};

	enum Modifier
	{
		WSI_MODIFIER_SHIFT = 0x0001,
		WSI_MODIFIER_CONTROL = 0x0002,
		WSI_MODIFIER_ALT = 0x0004,
		WSI_MODIFIER_SUPER = 0x0008,
		WSI_MODIFIER_CAPS_LOCK = 0x0010,
		WSI_MODIFIER_NUM_LOCK = 0x0020,
		WSI_MODIFIER_LAST = WSI_MODIFIER_NUM_LOCK
	};

	enum CursorMode
	{
		WSI_CURSOR_MODE_NORMAL = 0x00034001,
		WSI_CURSOR_MODE_HIDDEN = 0x00034002,
		WSI_CURSOR_MODE_DISABLED = 0x00034003,
		WSI_CURSOR_MODE_LAST = WSI_CURSOR_MODE_DISABLED
	};

	//enum StructureType
	//{
	//	WSI_STRUCTURE_TYPE_SHELL_CREATE_INFO = 0
	//};
class Shell
{
public:
	VULKAN_HPP_CONSTEXPR Shell()
		:
		m_shell(VK_NULL_HANDLE) {}

	VULKAN_HPP_CONSTEXPR Shell(std::nullptr_t)
		:
		m_shell(VK_NULL_HANDLE) {}

	VULKAN_HPP_TYPESAFE_EXPLICIT Shell(WsiShell shell)
		:
		m_shell(shell) {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
	Shell & operator=(WsiShell shell)
	{
	  m_shell = shell;
	  return *this;
	}
#endif

	Shell &operator=(std::nullptr_t)
	{
		m_shell = VK_NULL_HANDLE;
		return *this;
	}

	bool operator==(Shell const &rhs) const
	{
		return m_shell == rhs.m_shell;
	}

	bool operator!=(Shell const &rhs) const
	{
		return m_shell != rhs.m_shell;
	}

	bool operator<(Shell const &rhs) const
	{
		return m_shell < rhs.m_shell;
	}

	VULKAN_HPP_TYPESAFE_EXPLICIT operator WsiShell() const
	{
		return m_shell;
	}

	explicit operator bool() const
	{
		return m_shell != VK_NULL_HANDLE;
	}

	bool operator!() const
	{
		return m_shell == VK_NULL_HANDLE;
	}

	private:
		WsiShell m_shell;
	};

static_assert( sizeof( Shell ) == sizeof( VkInstance ), "handle and wrapper have different size!" );

}

#endif // VULKAN_WSI_HPP
