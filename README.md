# Vulkan WSI
VWSI is a low level Window System Integration (WSI) written in C for Vulkan. A WSI platform specific instance manages a window, mouse, keyboard, touch, and joysticks, accessible though callbacks. Vulkan WSI shares the same API style as Vulkan, therefore it shares a large mount of creation boilerplate.

Vulkan WSI only provides surface information to Vulkan, device and instance creation is still required. Vulkan WSI is simpler than libraries such as GLFW, therefore some functions are limited.

# Platforms
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