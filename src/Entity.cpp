#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
//
#include <Map.hpp>
//
#include "Entity.hpp"

Entity::Entity( PointF const &position, Map &map ) :
	mPosition( position ),
	mSpeed({ 0, 0 }),
	mMap( map )
{

}

void Entity::draw( sf::RenderTarget &target, sf::RenderStates states ) const
{
	sf::Color color( 255, 255, 255, 128 );
	sf::Vertex vertices[ 4 ];

	vertices[ 0 ].color = color;
	vertices[ 1 ].color = color;
	vertices[ 2 ].color = color;
	vertices[ 3 ].color = color;

	vertices[ 0 ].position = sf::Vector2f( mPosition.x, mPosition.y );
	vertices[ 1 ].position = sf::Vector2f( mPosition.x + 1, mPosition.y );
	vertices[ 2 ].position = sf::Vector2f( mPosition.x + 1, mPosition.y + 1 );
	vertices[ 3 ].position = sf::Vector2f( mPosition.x, mPosition.y + 1 );

	target.draw( vertices, 4, sf::Quads, states );
}

void Entity::simulate()
{
	PointF delta = { 0, 0 };
	delta += force({ -1, -1 });
	delta += force({  0, -1 });
	delta += force({  1, -1 });
	delta += force({  1,  0 });
	delta += force({  1,  1 });
	delta += force({  0,  1 });
	delta += force({ -1,  1 });
	delta += force({ -1,  0 });
	accelerate( delta );
	move();
}

PointF const &Entity::getPosition() const
{
	return mPosition;
}

PointI Entity::getMapPosition() const
{
	return mPosition + PointF( 0.5, 0.5 );
}

void Entity::bounce()
{
	mSpeed *= PointF( -0.75, -0.75 );
}

PointF Entity::force( PointI const &direction )
{
	PointF delta = { 0, 0 };
	float valDelta = 0;
	valDelta = mMap[{ getMapPosition() + direction }].val - mMap[ getMapPosition()].val;
	if( direction.x > 0 )
	{
		delta.x -= valDelta;
	}
	if( direction.x < 0 )
	{
		delta.x += valDelta;
	}
	if( direction.y > 0 )
	{
		delta.y -= valDelta;
	}
	if( direction.y < 0 )
	{
		delta.y += valDelta;
	}
	if( direction.y != 0 && direction.x != 0 )
	{
		delta /= { std::sqrt( 2 ), std::sqrt( 2 )};
	}
	return delta;
}

void Entity::accelerate( PointF const &vector )
{
	mSpeed += vector;
}

void Entity::move()
{
	PointF newPosition = { mPosition.x + mSpeed.x, mPosition.y + mSpeed.y };
	PointI newMapPosition = newPosition + PointF( 0.5, 0.5 );

	if( mMap.inBounds( newMapPosition ) && !mMap[ newMapPosition ].wall )
	{
		mMap[ getMapPosition()].val += 0.01;
		mPosition = newPosition;
		mMap[ getMapPosition()].val -= 0.01;
	}
	else
	{
		bounce();
	}
}
