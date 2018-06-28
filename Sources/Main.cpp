#include <iostream>

#include "Shells/ShellWin32.hpp"

// https://github.com/LunarG/VulkanSamples/blob/master/Sample-Programs/Hologram/Shell.h
// https://github.com/LunarG/VulkanSamples/blob/master/Sample-Programs/Hologram/Shell.cpp
// https://github.com/LunarG/VulkanSamples/blob/master/Sample-Programs/Hologram/ShellWin32.h
// https://github.com/LunarG/VulkanSamples/blob/master/Sample-Programs/Hologram/ShellWin32.cpp

int main(int argc, char **argv)
{
	auto shell = new ShellWin32();

	while (!shell->IsClosed())
	{
		shell->Update();
	}

	delete shell;

	// Pauses the console.
	std::cin.get();
	return 0;
}