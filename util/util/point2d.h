#ifndef POINT2D_H
#define POINT2D_H

#include <util/number.h>

template<typename U, typename V>
struct point2d_c
{
	typedef U u;
	typedef V v;
};

template<typename U, typename V>
std::ostream& operator<<(std::ostream& os, point2d_c<U, V>)
{
	return os << '[' << U().get() << ", " << V().get() << ']';
}

#define POINT_MAKE(U, V) point2d_c<NUMBER_MAKE(U), NUMBER_MAKE(V)>
#define POINT_GET_U(point) NUMBER_GET_TYPE(point::u)
#define POINT_GET_V(point) NUMBER_GET_TYPE(point::v)

#endif // !POINT2D_H