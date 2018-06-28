#include "ShellWin32.hpp"

LRESULT ShellWin32::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto shell = reinterpret_cast<ShellWin32 *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if (!shell)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return shell->HandleMessage(uMsg, wParam, lParam);
}

LRESULT ShellWin32::HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		UINT width = LOWORD(lparam);
		UINT height = HIWORD(lparam);
		m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);

		if (m_fullscreen)
		{
			m_fullscreenWidth = width;
			m_fullscreenHeight = height;
		}
		else
		{
			m_windowWidth = width;
			m_windowHeight = height;
		}

	//	ResizeSwapchain(width, height);
	}
		break;
	case WM_KEYDOWN:
	//	switch (wparam)
	//	{
	//	case VK_ESCAPE:
	//		break;
	//	}
		break;
	case WM_SETFOCUS:
		m_focused = true;
		break;
	case WM_KILLFOCUS:
		m_focused = false;
		break;
	case WM_CLOSE:
		m_closed = false;
		break;
	case WM_DESTROY:
		m_closed = false;
		break;
	default:
		return DefWindowProc(hwnd_, msg, wparam, lparam);
		break;
	}

	return 0;
}

ShellWin32::ShellWin32() :
	hinstance_(nullptr),
	hwnd_(nullptr),
	hmodule_(nullptr)
{
	hinstance_ = GetModuleHandle(nullptr);

	WNDCLASSEX win_class = {};
	win_class.cbSize = sizeof(WNDCLASSEX);
	win_class.style = CS_HREDRAW | CS_VREDRAW;
	win_class.lpfnWndProc = WindowProc;
	win_class.hInstance = hinstance_;
	win_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
	win_class.lpszClassName = "Shell";
	RegisterClassEx(&win_class);

	const DWORD win_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_OVERLAPPEDWINDOW;

	RECT win_rect = {0, 0, m_windowWidth, m_windowHeight};
	AdjustWindowRect(&win_rect, win_style, false);

	hwnd_ = CreateWindowEx(WS_EX_APPWINDOW, "Shell", m_title.c_str(), win_style, 0, 0,
		win_rect.right - win_rect.left, win_rect.bottom - win_rect.top, nullptr, nullptr, hinstance_, nullptr);

	SetForegroundWindow(hwnd_);
	SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR) this);
}

ShellWin32::~ShellWin32()
{
	CloseWindow(hwnd_);
	FreeLibrary(hmodule_);
	DestroyWindow(hwnd_);
}

void ShellWin32::Update()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			m_closed = true;
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void ShellWin32::SetWindowSize(const int &width, const int &height)
{
	SetWindowPos(hwnd_, hwnd_, width, height, -1, -1, SWP_NOSIZE | SWP_NOZORDER);
}

void ShellWin32::SetTitle(const std::string &title)
{
//	SetWindowText(hwnd_, LPCSTR(title));
}

void ShellWin32::SetIcon(const std::string &filename)
{
}

void ShellWin32::SetFullscreen(const bool &fullscreen)
{
}
