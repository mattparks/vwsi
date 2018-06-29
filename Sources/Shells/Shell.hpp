#pragma once

#include <array>
#include <string>
#include "KeyCodes.hpp"

namespace wsi
{
	typedef std::array<bool, MODIFIER_LAST> Modifiers;
	typedef void (* PositionCallback)(int, int);
	typedef void (* SizeCallback)(int, int);
	typedef void (* FocusCallback)(bool);
	typedef void (* IconifyCallback)(bool);
	typedef void (* CloseCallback)();
	typedef void (* CursorPositionCallback)(float, float);
	typedef void (* CursorEnterCallback)(bool);
	typedef void (* CursorScrollCallback)(float, float);
	typedef void (* KeyCallback)(Key, Action, Modifiers);
	typedef void (* MouseButtonCallback)(MouseButton, Action);
	typedef void (* TouchCallback)(float, float, Action);
	typedef void (* JoystickConnectCallback)(JoystickPort, std::string, bool);
	typedef void (* JoystickButtonCallback)(JoystickPort, int, Action);
	typedef void (* JoystickAxisCallback)(JoystickPort, int, float);

	/// <summary>
	/// A class that wraps window and input functionality.
	/// </summary>
	class Shell
	{
	protected:
		int m_positionX;
		int m_positionY;
		int m_width;
		int m_height;
		bool m_focused;
		bool m_iconified;
		bool m_closed;

		std::string m_title;
		bool m_fullscreen;


		PositionCallback m_callbackPosition;
		SizeCallback m_callbackSize;
		FocusCallback m_callbackFocus;
		IconifyCallback m_callbackIconify;
		CloseCallback m_callbackClose;
		CursorPositionCallback m_callbackCursorPosition;
		CursorEnterCallback m_callbackCursorEnter;
		CursorScrollCallback m_callbackCursorScroll;
		KeyCallback m_callbackKey;
		MouseButtonCallback m_callbackMouseButton;
		TouchCallback m_callbackTouch;
		JoystickConnectCallback m_callbackJoystickConnect;
		JoystickButtonCallback m_callbackJoystickButton;
		JoystickAxisCallback m_callbackJoystickAxis;
	public:
		Shell(const int &width, const int &height, const std::string &title, const bool &resizable);

		~Shell();

		void Update() = 0;

		/// <summary>
		/// Gets the width of the display in pixels.
		/// </summary>
		/// <returns> The width of the display. </returns>
		int GetWidth() { return m_fullscreen ? m_fullscreenWidth : m_windowWidth; }

		/// <summary>
		/// Sets the width of the display in pixels.
		/// </summary>
		/// <param name="width"> The new width in pixels. </param>
		void SetWidth(const int &width) { SetWindowSize(width, GetHeight()); }

		/// <summary>
		/// Gets the height of the display in pixels.
		/// </summary>
		/// <returns> The height of the display. </returns>
		int GetHeight() { return m_fullscreen ? m_fullscreenHeight : m_windowHeight; }

		/// <summary>
		/// Sets the height of the display in pixels.
		/// </summary>
		/// <param name="height"> The new height in pixels. </param>
		void SetHeight(const int &height) { SetWindowSize(GetWidth(), height); }

		/// <summary>
		/// Sets window size to a new size.
		/// </summary>
		/// <param name="width"> The new width in pixels. </param>
		/// <param name="height"> The new height in pixels. </param>
		void SetWindowSize(const int &width, const int &height) = 0;

		/// <summary>
		/// Gets the non-fullscreen width of the display in pixels.
		/// </summary>
		/// <returns> The width of the display. </returns>
		int GetWindowWidth() const { return m_windowWidth; }

		/// <summary>
		/// Gets the non-fullscreen height of the display in pixels.
		/// </summary>
		/// <returns> The height of the display. </returns>
		int GetWindowHeight() const { return m_windowHeight; }

		/// <summary>
		/// Gets the aspect ratio between the displays width and height.
		/// </summary>
		/// <returns> The aspect ratio. </returns>
		float GetAspectRatio() const { return m_aspectRatio; }

		/// <summary>
		/// Gets the window's title.
		/// </summary>
		/// <returns> The window's title. </returns>
		std::string GetTitle() const { return m_title; }

		/// <summary>
		/// Sets window title
		/// </summary>
		/// <param name="title"> The new title. </param>
		void SetTitle(const std::string &title) = 0;

		/// <summary>
		/// Gets the window's icon file.
		/// </summary>
		/// <returns> The window's icon file. </returns>
		std::string GetIcon() const { return m_iconPath; }

		/// <summary>
		/// Sets window icon image.
		/// </summary>
		/// <param name="filename"> The new icon file. </param>
		void SetIcon(const std::string &filename);

		/// <summary>
		/// Gets weather the display is fullscreen or not.
		/// </summary>
		/// <returns> Fullscreen or windowed. </returns>
		bool IsFullscreen() const { return m_fullscreen; }

		/// <summary>
		/// Sets the display to be fullscreen or windowed.
		/// </summary>
		/// <param name="fullscreen"> Weather or not to be fullscreen. </param>
		void SetFullscreen(const bool &fullscreen);

		/// <summary>
		/// Gets if the GLFW display is closed.
		/// </summary>
		/// <returns> If the GLFW display is closed. </returns>
		bool IsClosed() const { return m_closed; }

		/// <summary>
		/// Gets if the GLFW display is selected.
		/// </summary>
		/// <returns> If the GLFW display is selected. </returns>
		bool IsFocused() const { return m_focused; }

		/// <summary>
		/// Gets the windows Y position of the display in pixels.
		/// </summary>
		/// <returns> The windows x position. </returns>
		int GetPositionX() const { return m_positionX; }

		/// <summary>
		/// Gets the windows Y position of the display in pixels.
		/// </summary>
		/// <returns> The windows Y position. </returns>
		int GetPositionY() const { return m_positionY; }

		/// <summary>
		/// Gets the windows is minimized.
		/// </summary>
		/// <returns> If the window is minimized. </returns>
		bool IsIconified() const { return m_iconified; }

		void SetPositionCallback(PositionCallback callbackPosition) { m_callbackPosition = callbackPosition; }
		
		void SetSizeCallback(SizeCallback callbackSize) { m_callbackSize = callbackSize; }
		
		void SetFocusCallback(FocusCallback callbackFocus) { m_callbackFocus = callbackFocus; }
		
		void SetIconifyCallback(IconifyCallback callbackIconify) { m_callbackIconify = callbackIconify; }
		
		void SetCloseCallback(CloseCallback callbackClose) { m_callbackClose = callbackClose; }
		
		void SetCursorPositionCallback(CursorPositionCallback callbackCursorPosition) { m_callbackCursorPosition = callbackCursorPosition; }
		
		void SetCursorEnterCallback(CursorEnterCallback callbackCursorEnter) { m_callbackCursorEnter = callbackCursorEnter; }
		
		void SetCursorScrollCallback(CursorScrollCallback callbackCursorScroll) { m_callbackCursorScroll = callbackCursorScroll; }

		void SetKeyCallback(KeyCallback callbackKey) { m_callbackKey = callbackKey; }

		void SetMouseButtonCallback(MouseButtonCallback callbackMouseButton) { m_callbackMouseButton = callbackMouseButton; }

		void SetTouchCallback(TouchCallback callbackTouch) { m_callbackTouch = callbackTouch; }

		void SetJoystickConnectCallback(JoystickConnectCallback callbackJoystickConnect) { m_callbackJoystickConnect = callbackJoystickConnect; }

		void SetJoystickButtonCallback(JoystickButtonCallback callbackJoystickButton) { m_callbackJoystickButton = callbackJoystickButton; }

		void SetJoystickAxisCallback(JoystickAxisCallback callbackJoystickAxis) { m_callbackJoystickAxis = callbackJoystickAxis; }
	};
}
