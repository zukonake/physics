#include <cmath>
//
#include <SFML/Graphics/RenderTarget.hpp>
//
#include "Tile.hpp"

Tile::Tile( float const &val, bool const &wall, PointI const &velocity ) :
	val( val ),
	wall( wall ),
	velocity( velocity )
{
}

void Tile::draw( sf::RenderTarget &target, sf::RenderStates states ) const
{
	sf::Color color = sf::Color::Black;
	sf::Vertex vertices[ 4 ];

	if( wall )
	{
		color = { 128, 128, 128, 255 };
	}
	else if( val > 0 )
	{
		color.r = 255 * sqrt( sqrt( val ));
		color.g = 255 * sqrt( val );
		color.b = 255 * val;
	}
	else if( val < 0 )
	{
		float absolute = std::fabs( val );
		color.r = 255 * sqrt( absolute );
		color.g = 255 * absolute;
		color.b = 255 * sqrt( sqrt( absolute ));
	}
	vertices[ 0 ].color = color;
	vertices[ 1 ].color = color;
	vertices[ 2 ].color = color;
	vertices[ 3 ].color = color;

	vertices[ 0 ].position = sf::Vector2f( 0, 0 );
	vertices[ 1 ].position = sf::Vector2f( 1, 0 );
	vertices[ 2 ].position = sf::Vector2f( 1, 1 );
	vertices[ 3 ].position = sf::Vector2f( 0, 1 );

	target.draw( vertices, 4, sf::Quads, states );
}
