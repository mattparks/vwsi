# Vulkan WSI
VWSI is a low level Window System Integration (WSI) written in C for Vulkan. A WSI platform specific instance manages a window, mouse, keyboard, touch, and joysticks, accessible though callbacks. Vulkan WSI shares the same API style as Vulkan, therefore it shares a large mount of creation boilerplate.

Vulkan WSI only provides surface information to Vulkan, device and instance creation is still required. Vulkan WSI is simpler than libraries such as GLFW, therefore some functions are limited.

Support for Windows, Linux, MacOS, Android, IOS, etc...