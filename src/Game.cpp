#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
//
#include "Game.hpp"

Game::Game() :
	mSimulating( false ),
	mScale( 4 ),
	mMap({ 200, 150 }),
	mWindow( sf::VideoMode( 800, 600 ), "Physics", sf::Style::Titlebar )
{
	mTransform.scale( mScale, mScale );
}

void Game::start()
{
	while( mWindow.isOpen())
	{
		render();
		handleInput();
		if( mSimulating )
		{
			mMap.simulate();
		}
	}
}

void Game::render()
{
	mWindow.clear();
	mWindow.draw( mMap, mTransform );
	mWindow.display();
}

void Game::handleInput()
{
	sf::Event event;
	while( mWindow.pollEvent( event ))
	{
		switch( event.type )
		{
		case sf::Event::Closed:
			mWindow.close();
			break;

		case sf::Event::KeyPressed:
			if( event.key.code == sf::Keyboard::Q )
			{
				mSimulating = not mSimulating;
			}
		default:
		break;
		}
	}
	if( sf::Keyboard::isKeyPressed( sf::Keyboard::E ))
	{
		auto mouse = sf::Mouse::getPosition( mWindow );
		mouse /= mScale;
		mMap.createEntity({ static_cast< float >( mouse.x ), static_cast< float >( mouse.y )});
	}
	if( sf::Mouse::isButtonPressed( sf::Mouse::Left ))
	{
		auto mouse = sf::Mouse::getPosition( mWindow );
		mouse /= mScale;
		mMap.brush({ 0.1, false, { 0, 0 }}, { mouse.x, mouse.y }, 5 );
	}
	else if( sf::Mouse::isButtonPressed( sf::Mouse::Right ))
	{
		auto mouse = sf::Mouse::getPosition( mWindow );
		mouse /= mScale;
		mMap.brush({ -0.1, false, { 0, 0 }}, { mouse.x, mouse.y }, 5 );
	}
	else if( sf::Mouse::isButtonPressed( sf::Mouse::Middle ))
	{
		auto mouse = sf::Mouse::getPosition( mWindow );
		mouse /= mScale;
		mMap.brush({ 0, true, { 0, 0 }}, { mouse.x, mouse.y }, 1 );
	}
}
