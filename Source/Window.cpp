#include "Headers/Window.h"

Window::Window()
{

}

Window::~Window()
{
	delete _renderWindow;
}

void Window::Init(string title, int x, int y)
{
	_renderWindow = new sf::RenderWindow(sf::VideoMode(x, y), title);
	_renderWindow->setFramerateLimit(60);
}

sf::RenderWindow* Window::Get()
{
	return _renderWindow;
}