#include "Shell.hpp"

namespace wsi
{
	Shell::Shell(const int &width, const int &height, const std::string &title, const bool &resizable) :
		m_width(width),
		m_height(height),
		m_fullscreenWidth(0),
		m_fullscreenHeight(0),
		m_aspectRatio(1.5f),
		m_title("Hello World"),
		m_iconPath(""),
		m_antialiasing(true),
		m_fullscreen(false),
		m_closed(false),
		m_focused(true),
		m_windowPosX(0),
		m_windowPosY(0),
		m_iconified(false)
	{
	}

	Shell::~Shell()
	{
	}
}
