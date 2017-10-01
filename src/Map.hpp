#pragma once

#include <vector>
#include <cmath>
//
#include <SFML/Graphics/Drawable.hpp>
//
#include <Point.hpp>
#include <Tile.hpp>
#include <Entity.hpp>

class Map : public sf::Drawable
{
	public:
	Map( PointI const &size );

	Tile const &operator[]( PointI const &point ) const;
	Tile &operator[]( PointI const &point );

	virtual void draw( sf::RenderTarget &target, sf::RenderStates states ) const override;

	bool inBounds( PointI const &point ) const;
	void brush( Tile const &tile, PointI const &point, int const &radius );
	void createEntity( PointF const &point );
	void simulate();
	Entity *getEntityOn( PointI const &point );
	private:
	float sumNeighbors( PointI const &point );
	int getNeighbors( PointI const &point );

	PointI mSize;
	std::vector< std::vector< Tile >> mValue;
	std::vector< Entity > mEntities;
};

