#include "Shell.hpp"

Shell::Shell() :
	m_windowWidth(1080),
	m_windowHeight(720),
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
