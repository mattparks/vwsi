#ifndef VULKAN_WSI_H
#define VULKAN_WSI_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <vulkan/vulkan.h>

#if defined(_WIN32)
#  if defined(_MSC_VER)
#    if defined(WSI_STATICLIB)
#      define WSIAPI
#    else
#      if defined(WSI_EXPORTS)
#        define WSIAPI __declspec(dllexport)
#      else
#        define WSIAPI __declspec(dllimport)
#      endif
#	 endif
#  else
#    if defined(WSI_STATICLIB)
#      define WSIAPI
#    else
#      if defined(WSI_EXPORTS)
#        define WSIAPI __attribute__ ((dllexport))
#      else
#        define WSIAPI __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#else
#  define WSIAPI __attribute__ ((visibility ("default")))
#endif

typedef enum WsiResult
{
	WSI_SUCCESS = 0
} WsiResult;

typedef enum WsiKey
{
	KEY_UNKNOWN = -1,
	KEY_SPACE = 32,
	KEY_APOSTROPHE = 39,
	KEY_COMMA = 44,
	KEY_MINUS = 45,
	KEY_PERIOD = 46,
	KEY_SLASH = 47,
	KEY_0 = 48,
	KEY_1 = 49,
	KEY_2 = 50,
	KEY_3 = 51,
	KEY_4 = 52,
	KEY_5 = 53,
	KEY_6 = 54,
	KEY_7 = 55,
	KEY_8 = 56,
	KEY_9 = 57,
	KEY_SEMICOLON = 59,
	KEY_EQUAL = 61,
	KEY_A = 65,
	KEY_B = 66,
	KEY_C = 67,
	KEY_D = 68,
	KEY_E = 69,
	KEY_F = 70,
	KEY_G = 71,
	KEY_H = 72,
	KEY_I = 73,
	KEY_J = 74,
	KEY_K = 75,
	KEY_L = 76,
	KEY_M = 77,
	KEY_N = 78,
	KEY_O = 79,
	KEY_P = 80,
	KEY_Q = 81,
	KEY_R = 82,
	KEY_S = 83,
	KEY_T = 84,
	KEY_U = 85,
	KEY_V = 86,
	KEY_W = 87,
	KEY_X = 88,
	KEY_Y = 89,
	KEY_Z = 90,
	KEY_LEFT_BRACKET = 91,
	KEY_BACKSLASH = 92,
	KEY_RIGHT_BRACKET = 93,
	KEY_GRAVE_ACCENT = 96,
	KEY_WORLD_1 = 161,
	KEY_WORLD_2 = 162,
	KEY_ESCAPE = 256,
	KEY_ENTER = 257,
	KEY_TAB = 258,
	KEY_BACKSPACE = 259,
	KEY_INSERT = 260,
	KEY_DELETE = 261,
	KEY_RIGHT = 262,
	KEY_LEFT = 263,
	KEY_DOWN = 264,
	KEY_UP = 265,
	KEY_PAGE_UP = 266,
	KEY_PAGE_DOWN = 267,
	KEY_HOME = 268,
	KEY_END = 269,
	KEY_CAPS_LOCK = 280,
	KEY_SCROLL_LOCK = 281,
	KEY_NUM_LOCK = 282,
	KEY_PRINT_SCREEN = 283,
	KEY_PAUSE = 284,
	KEY_F1 = 290,
	KEY_F2 = 291,
	KEY_F3 = 292,
	KEY_F4 = 293,
	KEY_F5 = 294,
	KEY_F6 = 295,
	KEY_F7 = 296,
	KEY_F8 = 297,
	KEY_F9 = 298,
	KEY_F10 = 299,
	KEY_F11 = 300,
	KEY_F12 = 301,
	KEY_F13 = 302,
	KEY_F14 = 303,
	KEY_F15 = 304,
	KEY_F16 = 305,
	KEY_F17 = 306,
	KEY_F18 = 307,
	KEY_F19 = 308,
	KEY_F20 = 309,
	KEY_F21 = 310,
	KEY_F22 = 311,
	KEY_F23 = 312,
	KEY_F24 = 313,
	KEY_F25 = 314,
	KEY_KP_0 = 320,
	KEY_KP_1 = 321,
	KEY_KP_2 = 322,
	KEY_KP_3 = 323,
	KEY_KP_4 = 324,
	KEY_KP_5 = 325,
	KEY_KP_6 = 326,
	KEY_KP_7 = 327,
	KEY_KP_8 = 328,
	KEY_KP_9 = 329,
	KEY_KP_DECIMAL = 330,
	KEY_KP_DIVIDE = 331,
	KEY_KP_MULTIPLY = 332,
	KEY_KP_SUBTRACT = 333,
	KEY_KP_ADD = 334,
	KEY_KP_ENTER = 335,
	KEY_KP_EQUAL = 336,
	KEY_LEFT_SHIFT = 340,
	KEY_LEFT_CONTROL = 341,
	KEY_LEFT_ALT = 342,
	KEY_LEFT_SUPER = 343,
	KEY_RIGHT_SHIFT = 344,
	KEY_RIGHT_CONTROL = 345,
	KEY_RIGHT_ALT = 346,
	KEY_RIGHT_SUPER = 347,
	KEY_MENU = 348,
	KEY_LAST = KEY_MENU
} WsiKey;

typedef enum WsiMouse
{
	MOUSE_1 = 0,
	MOUSE_2 = 1,
	MOUSE_3 = 2,
	MOUSE_4 = 3,
	MOUSE_5 = 4,
	MOUSE_6 = 5,
	MOUSE_7 = 6,
	MOUSE_8 = 7,
	MOUSE_LAST = MOUSE_8,
	MOUSE_LEFT = MOUSE_1,
	MOUSE_RIGHT = MOUSE_2,
	MOUSE_MIDDLE = MOUSE_3
} WsiMouse;

typedef enum WsiJoystick
{
	JOYSTICK_1 = 0,
	JOYSTICK_2 = 1,
	JOYSTICK_3 = 2,
	JOYSTICK_4 = 3,
	JOYSTICK_5 = 4,
	JOYSTICK_6 = 5,
	JOYSTICK_7 = 6,
	JOYSTICK_8 = 7,
	JOYSTICK_9 = 8,
	JOYSTICK_10 = 9,
	JOYSTICK_11 = 10,
	JOYSTICK_12 = 11,
	JOYSTICK_13 = 12,
	JOYSTICK_14 = 13,
	JOYSTICK_15 = 14,
	JOYSTICK_16 = 15,
	JOYSTICK_LAST = JOYSTICK_16
} WsiJoystick;

typedef enum WsiAction
{
	ACTION_PRESS = 0x0001,
	ACTION_RELEASE = 0x0002,
	ACTION_REPEAT = 0x0003,
	ACTION_LAST = ACTION_REPEAT
} WsiAction;

typedef enum WsiModifier
{
	MODIFIER_SHIFT = 0x0001,
	MODIFIER_CONTROL = 0x0002,
	MODIFIER_ALT = 0x0004,
	MODIFIER_SUPER = 0x0008,
	MODIFIER_CAPS_LOCK = 0x0010,
	MODIFIER_NUM_LOCK = 0x0020,
	MODIFIER_LAST = MODIFIER_NUM_LOCK
} WsiModifier;

typedef struct WsiInstance_T *WsiInstance;

typedef void (* PositionCallback)(WsiInstance, uint32_t, uint32_t);
typedef void (* SizeCallback)(WsiInstance, uint32_t, uint32_t, bool);
typedef void (* FocusCallback)(WsiInstance, bool);
typedef void (* IconifyCallback)(WsiInstance, bool);
typedef void (* CloseCallback)(WsiInstance);
typedef void (* CursorPositionCallback)(WsiInstance, float, float);
typedef void (* CursorEnterCallback)(WsiInstance, bool);
typedef void (* CursorScrollCallback)(WsiInstance, float, float);
typedef void (* KeyCallback)(WsiInstance, WsiKey, WsiAction, uint32_t, WsiModifier *);
typedef void (* MouseCallback)(WsiInstance, WsiMouse, WsiAction);
typedef void (* TouchCallback)(WsiInstance, float, float, WsiAction);
typedef void (* JoystickConnectCallback)(WsiInstance, WsiJoystick, const char *, bool);
typedef void (* JoystickButtonCallback)(WsiInstance, WsiJoystick, uint32_t, WsiAction);
typedef void (* JoystickAxisCallback)(WsiInstance, WsiJoystick, uint32_t, float);

typedef struct WsiCallbacks
{
	PositionCallback position;
	SizeCallback size;
	FocusCallback focus;
	IconifyCallback iconify;
	CloseCallback close;
	CursorPositionCallback cursorPosition;
	CursorEnterCallback cursorEnter;
	CursorScrollCallback cursorScroll;
	KeyCallback key;
	MouseCallback mouse;
	TouchCallback touch;
	JoystickConnectCallback joystickConnect;
	JoystickButtonCallback joystickButton;
	JoystickAxisCallback joystickAxis;
} WsiCallbacks;

typedef struct WsiIcon
{
	uint32_t width;
	uint32_t height;
	unsigned char* pixels;
} WsiIcon;

typedef struct WsiInstanceCreateInfo
{
	const WsiCallbacks *pCallbacks;
	const WsiIcon *pIcon;
	uint32_t width;
	uint32_t height;
	bool resizable;
	const char *title;
} WsiInstanceCreateInfo;

WSIAPI WsiResult wsiCreateInstance(const WsiInstanceCreateInfo *pCreateInfo, WsiInstance *pInstance);

WSIAPI void wsiDestroyInstance(WsiInstance instance);

WSIAPI WsiResult wsiEnumerateInstanceExtensions(WsiInstance instance, uint32_t *pPropertyCount, const char **pProperties);

WSIAPI VkResult wsiCreateSurface(VkInstance vkInstance, WsiInstance wsiInstance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface);

WSIAPI WsiResult wsiPollEvents(WsiInstance instance);

WSIAPI WsiResult wsiCmdSetSize(WsiInstance instance, uint32_t width, uint32_t height);

WSIAPI WsiResult wsiCmdSetTitle(WsiInstance instance, const char *title);

WSIAPI WsiResult wsiCmdSetIcon(WsiInstance instance, const WsiIcon *icon);

WSIAPI WsiResult wsiCmdSetFullscreen(WsiInstance instance, bool fullscreen);

#ifdef __cplusplus
}
#endif

#endif //VULKAN_WSI_H
