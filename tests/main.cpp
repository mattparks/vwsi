#include <iostream>
#include <wsi/vulkan_wsi.h>
#include <vector>

std::string StringifyResultWsi(const WsiResult &result)
{
	switch (result)
	{
	case WSI_SUCCESS:
		return "Success";
	default:
		return "ERROR: UNKNOWN WSI ERROR";
	}
}
void ErrorWsi(const WsiResult &result)
{
	if (result >= 0)
	{
		return;
	}

	std::string failure = StringifyResultWsi(result);

	fprintf(stderr, "WSI error: %s, %i\n", failure.c_str(), result);
#ifdef FL_BUILD_WINDOWS
	MessageBox(nullptr, failure.c_str(), "WSI Error", 0);
#endif
	throw std::runtime_error("WSI runtime error.");
}

bool m_closed;

void CallbackPosition(WsiInstance instance, uint32_t x, uint32_t y)
{
}

void CallbackSize(WsiInstance instance, uint32_t width, uint32_t height, bool fullscreen)
{
}

void CallbackFocus(WsiInstance instance, bool focused)
{
}

void CallbackIconify(WsiInstance instance, bool iconified)
{
}

void CallbackClose(WsiInstance instance)
{
	m_closed = true;
}

void CallbackCursorPosition(WsiInstance instance, float x, float y)
{
}

void CallbackCursorEnter(WsiInstance instance, bool entered)
{
}

void CallbackCursorScroll(WsiInstance instance, float x, float y)
{
}

void CallbackKey(WsiInstance instance, WsiKey key, WsiAction action, uint32_t modsCount, WsiModifier *mods)
{
}

void CallbackMouseButton(WsiInstance instance, WsiMouse mouse, WsiAction action)
{
}

void CallbackTouch(WsiInstance instance, float x, float y, WsiAction action)
{
}

void CallbackJoystickConnect(WsiInstance instance, WsiJoystick port, const char *name, bool connected)
{
}

void CallbackJoystickButton(WsiInstance instance, WsiJoystick port, uint32_t button, WsiAction action)
{
}

void CallbackJoystickAxis(WsiInstance instance, WsiJoystick port, uint32_t axis, float amount)
{
}

WsiInstance m_shell;

int main(int argc, char **argv)
{
	WsiCallbacks callbacks = {};
	callbacks.position = CallbackPosition;
	callbacks.size = CallbackSize;
	callbacks.focus = CallbackFocus;
	callbacks.iconify = CallbackIconify;
	callbacks.close = CallbackClose;
	callbacks.cursorPosition = CallbackCursorPosition;
	callbacks.cursorEnter = CallbackCursorEnter;
	callbacks.cursorScroll = CallbackCursorScroll;
	callbacks.key = CallbackKey;
	callbacks.mouse = CallbackMouseButton;
	callbacks.touch = CallbackTouch;
	callbacks.joystickConnect = CallbackJoystickConnect;
	callbacks.joystickButton = CallbackJoystickButton;
	callbacks.joystickAxis = CallbackJoystickAxis;

	WsiIcon icon = {};
	icon.width = 0;
	icon.height = 0;
	icon.pixels = nullptr;

	WsiInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.pCallbacks = &callbacks;
	instanceCreateInfo.pIcon = &icon;
	instanceCreateInfo.width = 1080;
	instanceCreateInfo.height = 720;
	instanceCreateInfo.resizable = true;
	instanceCreateInfo.title = "Hello World";

	ErrorWsi(wsiCreateInstance(&instanceCreateInfo, &m_shell));

//	wsiCmdSetSize(m_shell, 1920, 1080);
//	wsiCmdSetTitle(m_shell, "New Title");
//	wsiCmdSetIcon(m_shell, &icon);
//	wsiCmdSetFullscreen(m_shell, true);

	uint32_t instanceExtensionsCount;
	wsiEnumerateInstanceExtensions(m_shell, &instanceExtensionsCount, nullptr);
	std::vector<const char *> instanceExtensions(instanceExtensionsCount);
	wsiEnumerateInstanceExtensions(m_shell, &instanceExtensionsCount, instanceExtensions.data());

	// TODO: Vulkan stuff.

//	ErrorWsi(wsiCreateSurface(m_instance, m_shell, nullptr, &m_surface));

	while (!m_closed)
	{
		wsiPollEvents(m_shell);
	}

	wsiDestroyInstance(m_shell);

	// Pauses the console.
	std::cin.get();
	return 0;
}