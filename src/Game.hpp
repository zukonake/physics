#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
//
#include <Map.hpp>

class Game
{
	public:
	Game();

	void start();
	private:
	void render();
	void handleInput();

	bool mSimulating;
	int mScale;
	Map mMap;
	sf::Transform mTransform;
	sf::RenderWindow mWindow;
};
