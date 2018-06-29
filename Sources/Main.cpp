#include <iostream>

#include "Shells/ShellWin32.hpp"

using namespace wsi;

void CallbackPosition(int x, int y)
{
}

void CallbackSize(int width, int height)
{
}

void CallbackFocus(bool focused)
{
}

void CallbackIconify(bool iconified)
{
}

void CallbackClose()
{
}

void CallbackCursorPosition(float x, float y)
{
}

void CallbackCursorEnter(bool entered)
{
}

void CallbackCursorScroll(float x, float y)
{
}

void CallbackKey(Key key, Action action, Modifiers mods)
{
}

void CallbackMouseButton(MouseButton button, Action action)
{
}

void CallbackTouch(float x, float y, Action action)
{
}

void CallbackJoystickConnect(JoystickPort port, std::string name, bool connected)
{
}

void CallbackJoystickButton(JoystickPort port, int button, Action action)
{
}

void CallbackJoystickAxis(JoystickPort port, int axis, float amount)
{
}

int main(int argc, char **argv)
{
	auto shell = new Shell(1080, 720, "Hello World", true);
	shell->SetPositionCallback(CallbackPosition);
	shell->SetSizeCallback(CallbackSize);
	shell->SetFocusCallback(CallbackFocus);
	shell->SetIconifyCallback(CallbackIconify);
	shell->SetCloseCallback(CallbackClose);
	shell->SetCursorPositionCallback(CallbackCursorPosition);
	shell->SetCursorEnterCallback(CallbackCursorEnter);
	shell->SetCursorScrollCallback(CallbackCursorScroll);
	shell->SetKeyCallback(CallbackKey);
	shell->SetMouseButtonCallback(CallbackMouseButton);
	shell->SetTouchCallback(CallbackTouch);
	shell->SetJoystickConnectCallback(CallbackJoystickConnect);
	shell->SetJoystickButtonCallback(CallbackJoystickButton);
	shell->SetJoystickAxisCallback(CallbackJoystickAxis);

	while (!shell->IsClosed())
	{
		shell->Update();
	}

	delete shell;

	// Pauses the console.
	std::cin.get();
	return 0;
}