#pragma once

#include <windows.h>
#include "Shell.hpp"

class ShellWin32 :
	public Shell
{
private:
	HINSTANCE hinstance_;
	HWND hwnd_;

	HMODULE hmodule_;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam);
public:
	ShellWin32();

	~ShellWin32();

	void Update() override;

	void SetWindowSize(const int &width, const int &height) override;

	void SetTitle(const std::string &title) override;

	void SetIcon(const std::string &filename) override;

	void SetFullscreen(const bool &fullscreen) override;
};