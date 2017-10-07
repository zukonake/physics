#include <stdexcept>
#include <cmath>
//
#include <SFML/Graphics/RenderTarget.hpp>
//
#include "Map.hpp"

Map::Map( PointI const &size ) :
	mSize( size )
{
	for( int iY = 0; iY < mSize.y; iY++ )
	{
		mValue.emplace_back();
		for( int iX = 0; iX < mSize.x; iX++ )
		{
			mValue[ iY ].push_back({ 0, false, { 0, 0 }});
		}
	}
	for( int iX = 0; iX < mSize.x; iX++ )
	{
		mValue[ 0 ][ iX ] = { 0, true, { 0, 0 }};
		mValue[ mSize.y - 1 ][ iX ] = { 0, true, { 0, 0 }};
	}
	for( int iY = 0; iY < mSize.y; iY++ )
	{
		mValue[ iY ][ 0 ] = { 0, true, { 0, 0 }};
		mValue[ iY ][ mSize.x - 1 ] = { 0, true, { 0, 0 }};
	}
}

Tile const &Map::operator[]( PointI const &point ) const
{
	return mValue[ point.y ][ point.x ];
}

Tile &Map::operator[]( PointI const &point )
{
	return mValue[ point.y ][ point.x ];
}

void Map::draw( sf::RenderTarget &target, sf::RenderStates states ) const
{
	sf::RenderStates before = states;
	for( int iY = 0; iY < mSize.y; iY++ )
	{
		for( int iX = 0; iX < mSize.x; iX++ )
		{
			target.draw( mValue[ iY ][ iX ], states );
			states.transform.translate({ 1, 0 });
		}
		states.transform.translate({ -static_cast< float >( mSize.x ), 0 });
		states.transform.translate({ 0, 1 });
	}
	for( auto const &iEntity : mEntities )
	{
		target.draw( iEntity, before );
	}
}

bool Map::inBounds( PointI const &point ) const
{
	return point.x >= 0 && point.x < ( mSize.x ) && point.y >= 0 && point.y < ( mSize.y );
}

void Map::brush( Tile const &tile, PointI const &point, int const &radius )
{
	for( int iY = point.y - radius; iY < point.y + radius; iY++ )
	{
		for( int iX = point.x - radius; iX < point.x + radius; iX++ )
		{
			float distance = std::sqrt( std::pow( iX - point.x, 2 ) + std::pow( iY - point.y, 2 ));
			if( inBounds({ iX, iY }) && distance <= radius)
			{
				mValue[ iY ][ iX ].val += tile.val;
				mValue[ iY ][ iX ].wall |= tile.wall;
				mValue[ iY ][ iX ].velocity = tile.velocity;
				if( mValue[ iY ][ iX ].val > 1 )
				{
					mValue[ iY ][ iX ].val = 1;
				}
				if( mValue[ iY ][ iX ].val < -1 )
				{
					mValue[ iY ][ iX ].val = -1;
				}
			}
		}
	}
}

void Map::createEntity( PointF const &point )
{
	mEntities.emplace_back( point, *this );
}

void Map::simulate()
{
	for( auto &iEntity : mEntities )
	{
		iEntity.simulate();
	}
	Map newMap( mSize );
	for( int iY = 0; iY < mSize.y; iY++ )
	{
		for( int iX = 0; iX < mSize.x; iX++ )
		{
			PointI position = PointI( iX, iY );
			Tile &tile = newMap[ position ];
			PointF delta = { 0, 0 };
			delta += force( position, { -1, -1 });
			delta += force( position, {  0, -1 });
			delta += force( position, {  1, -1 });
			delta += force( position, {  1,  0 });
			delta += force( position, {  1,  1 });
			delta += force( position, {  0,  1 });
			delta += force( position, { -1,  1 });
			delta += force( position, { -1,  0 });
			tile.velocity += delta * PointF( 100, 100 );
			PointI newPosition = position + tile.velocity;
			if( operator[]( position ).wall )
			{
				tile.val = 0;
			}
			else
			{
				tile.val = ( sumNeighbors( position ) / getNeighbors( position ));
			}
			if( newPosition != position )
			{
				if( inBounds( newPosition ) && !operator[]( newPosition ).wall )
				{
					float distance = std::sqrt( std::pow( newPosition.x - position.x, 2 ) + std::pow( newPosition.y - position.y, 2 ));
					float change = ( operator[]( position ).val - operator[]( newPosition ).val) / 10;
					newMap[ position ].val = operator[]( position ).val - change;
					newMap[ newPosition ].val = operator[]( newPosition ).val + change;
				}
				else
				{
					newMap[ position ].velocity = operator[]( position ).velocity * PointI( -1, -1 );
				}
			}
		}
	}
	mValue.swap( newMap.mValue );
}

PointF Map::force( PointI const &source, PointI const &direction )
{
	if( !inBounds( source + direction ))
	{
		return { 0, 0 };
	}
	PointF delta = { 0, 0 };
	float valDelta = 0;
	valDelta = operator[]( source + direction ).val - operator[]( source ).val;
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

Entity *Map::getEntityOn( PointI const &point )
{
	for( auto &iEntity : mEntities )
	{
		if( iEntity.getMapPosition() == point )
		{
			return &iEntity;
		}
	}
	return nullptr;
}

float Map::sumNeighbors( PointI const &point )
{
	float neighbors = 0;
	for( int iY = point.y - 1; iY <= point.y + 1; iY++ )
	{
		for( int iX = point.x - 1; iX <= point.x + 1; iX++ )
		{
			if( inBounds({ iX, iY }))
			{
				if( mValue[ iY ][ iX ].wall == false )
				{
					neighbors += mValue[ iY ][ iX ].val;
				}
			}
		}
	}
	return neighbors;
}

int Map::getNeighbors( PointI const &point )
{
	int neighbors = 0;
	for( int iY = point.y - 1; iY <= point.y + 1; iY++ )
	{
		for( int iX = point.x - 1; iX <= point.x + 1; iX++ )
		{
			if( inBounds({ iX, iY }))
			{
				if( mValue[ iY ][ iX ].wall == false )
				{
					neighbors++;
				}
			}
			else
			{
				neighbors++;
			}
		}
	}
	return neighbors;
}
