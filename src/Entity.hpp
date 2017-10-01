#pragma once

#include <SFML/Graphics/Drawable.hpp>
//
#include <Point.hpp>

class Map;

class Entity : public sf::Drawable
{
	public:
	Entity( PointF const &position, Map &map );

	virtual void draw( sf::RenderTarget &target, sf::RenderStates states ) const override;

	void simulate();
	PointF const &getPosition() const;
	PointI getMapPosition() const;
	void bounce();
	private:
	PointF force( PointI const &direction );
	void accelerate( PointF const &vector );
	void move();

	PointF mPosition;
	PointF mSpeed;
	Map &mMap;
};
