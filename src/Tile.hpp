#pragma once

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Drawable.hpp>

struct Tile : public sf::Drawable
{
	Tile() = default;
	Tile( float const &val, bool const &wall );

	virtual void draw( sf::RenderTarget &target, sf::RenderStates states ) const override;

	float val;
	bool wall;
};
