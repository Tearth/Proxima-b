#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

class Window
{
public:
	Window();
	~Window();

	void Init(string title, int x, int y);
	sf::RenderWindow* Get();

private:
	sf::RenderWindow* _renderWindow;
};