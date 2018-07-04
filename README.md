# VWSI

[![Contribute!](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/mattparks/vwsi/issues)

VWSI is a low level Window System Integration (WSI) written in C for Vulkan. 

A WSI platform specific shell manages a window, mouse, keyboard, touch, and joysticks, accessible though callbacks (VWSI does not track-return values, they must be keep in your project). Vulkan WSI shares the same API style as Vulkan even going as far as to use a XML API, therefore it shares a large mount of boilerplate.

Vulkan WSI only provides surface information to Vulkan, device and instance creation is still required. Vulkan WSI is simpler than libraries such as GLFW, therefore some functions are limited.

## Builds
Windows   [![Build status](https://ci.appveyor.com/api/projects/status/4uhakf6tt78wov7o?svg=true)](https://ci.appveyor.com/project/Mattparks/vwsi)

Linux/MacOS   [![Build Status](https://travis-ci.org/Equilibrium-Games/Flounder.svg?branch=master)](https://travis-ci.org/mattparks/vwsi)

# Target Platforms 
- xlib: X Window System, Xlib client library
- xlib_xrandr: X Window System, Xlib client library, XRandR extension
- xcb: X Window System, Xcb client library
- wayland: Wayland display server protocol
- mir: Mir display server
- android: Android OS
- win32: Microsoft Win32 API (also refers to Win64 apps)
- vi: Nintendo Vi
- ios: Apple IOS
- macos: Apple MacOS

# Usage
Full example code can be found in `./test/main.cpp`.
```cpp
// Enumerates all connected monitors to the system.
uint32_t instanceMonitorCount;
wsiEnumerateMonitors(&instanceMonitorCount, nullptr);
assert(instanceMonitorCount != 0);
std::vector<WsiMonitor> monitors(instanceMonitorCount);
wsiEnumerateMonitors(&instanceMonitorCount, monitors.data());

// Selects the primary monitor, and retreves size info.
WsiMonitorProperties monitorProperties;
wsiGetMonitorProperties(monitors[0], &monitorProperties);

// Creates a list of callbacks, these can be changed later using wsiGetShellCallbacks.
WsiShellCallbacks shellCallbacks = {};
shellCallbacks.pfnPosition = CallbackPosition;
shellCallbacks.pfnSize = CallbackSize;
shellCallbacks.pfnFocus = CallbackFocus;
shellCallbacks.pfnClose = CallbackClose;
shellCallbacks.pfnCursorPosition = CallbackCursorPosition;
shellCallbacks.pfnCursorEnter = CallbackCursorEnter;
shellCallbacks.pfnCursorScroll = CallbackCursorScroll;
shellCallbacks.pfnChar = CallbackChar;
shellCallbacks.pfnKey = CallbackKey;
shellCallbacks.pfnMouseButton = CallbackMouseButton;
shellCallbacks.pfnTouch = CallbackTouch;
shellCallbacks.pfnJoystickConnect = CallbackJoystickConnect;
shellCallbacks.pfnJoystickButton = CallbackJoystickButton;
shellCallbacks.pfnJoystickAxis = CallbackJoystickAxis;

// Loads a window icon.
WsiImage icon = {};
icon.width = 48;
icon.height = 48;
icon.pixels = nullptr;
LoadImageWsi("windowIcon.png", &icon);

// Loads a custom cursor.
WsiImage cursor = {};
cursor.width = 36;
cursor.height = 36;
cursor.pixels = nullptr;
LoadImageWsi("windowCursor.png", &cursor);

// Create info used to create the shell, all values can be changed later on.
WsiShellCreateInfo shellCreateInfo = {};
shellCreateInfo.sType = WSI_STRUCTURE_TYPE_SHELL_CREATE_INFO;
shellCreateInfo.pCallbacks = &shellCallbacks;
shellCreateInfo.pIcon = &icon;
shellCreateInfo.pCursor = &cursor;
shellCreateInfo.width = 1080;
shellCreateInfo.height = 720;
shellCreateInfo.x = (monitorProperties.width - shellCreateInfo.width) / 2;
shellCreateInfo.y = (monitorProperties.height - shellCreateInfo.height) / 2;
shellCreateInfo.resizable = true;
shellCreateInfo.pName = "Hello World";

// Creates the shell object, VWSI will handle platform specific code.
WsiShell shell;
ErrorVk(wsiCreateShell(&shellCreateInfo, nullptr, &shell));

// Gets a list of extensions used in VkInstanceCreateInfo.enabledExtensionCount and VkInstanceCreateInfo.ppEnabledExtensionNames.
uint32_t instanceExtensionCount;
wsiEnumerateShellExtensions(shell, &instanceExtensionCount, nullptr);
assert(instanceExtensionCount != 0);
std::vector<const char *> instanceExtensions(instanceExtensionCount);
wsiEnumerateShellExtensions(shell, &instanceExtensionCount, instanceExtensions.data());

/// ... Vulkan ... //  

// Creates a Vulkan surface for the shell created above, platform specific code handled in VWSI.
VkSurfaceKHR surface;
ErrorVk(wsiCreateSurface(shell, instance, nullptr, &surface));

// Example usage of hot-swapping callbacks.
WsiShellCallbacks *callbacks;
wsiGetShellCallbacks(shell, &callbacks);
assert(callbacks != nullptr);
callbacks->pfnJoystickConnect = CallbackJoystickConnect;
callbacks->pfnJoystickButton = CallbackJoystickButton;
callbacks->pfnJoystickAxis = CallbackJoystickAxis;

// Example list of commands that can be run in WSI.
ErrorVk(wsiCmdSetResizable(shell, false));
ErrorVk(wsiCmdSetSize(shell, 720, 480));
ErrorVk(wsiCmdSetPosition(shell, 300, 200));
ErrorVk(wsiCmdSetFullscreen(shell, monitors[0], true));
ErrorVk(wsiCmdSetName(shell, "New Name"));
ErrorVk(wsiCmdSetIcon(shell, &icon));
ErrorVk(wsiCmdSetCursor(shell, &cursor));
ErrorVk(wsiCmdSetCursorMode(shell, WSI_CURSOR_MODE_DISABLED));
ErrorVk(wsiCmdSetCursorPos(shell, 100, 100));

while (!m_closed)
{
    // Poll changes in the window, this will look for inputs, and system changes. Callbacks provided to WSI are called here.
    ErrorVk(wsiCmdPollEvents(shell));
}

```