#pragma once

#include <Vector2D.hpp>

template< typename T >
using Point = geometry::Vector2D< T >;

typedef Point< int > PointI;
typedef Point< unsigned > PointU;
typedef Point< float > PointF;
