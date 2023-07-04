#ifndef VALUELIST_H
#define VALUELIST_H

// utility functions for typelists of values
#include <util/typelist.h>

#include <ostream>

template<typename values>
struct value_printer;

template<typename T, typename U>
struct value_printer<typelist<T, U>>
{
	friend std::ostream& operator<<(std::ostream& os, value_printer)
	{
		return os << T().get() << ", " << value_printer<U>();
	}
};

template<typename T>
struct value_printer<typelist<T, null_t>>
{
	friend std::ostream& operator<<(std::ostream& os, value_printer)
	{
		return os << T().get();
	}
};

template<typename Head, typename Tail, typename Next>
struct value_printer<typelist<typelist<Head, Tail>, Next> >
{
	friend std::ostream& operator<<(std::ostream& os, value_printer)
	{
		return os << '[' << value_printer<typelist<Head, Tail> >() << ']'
		 << '\n' << value_printer<Next>();
	}
};

template<typename Head, typename Tail>
struct value_printer<typelist<typelist<Head, Tail>, null_t> >
{
	friend std::ostream& operator<<(std::ostream& os, value_printer)
	{
		return os << '[' << value_printer<typelist<Head, Tail> >() << ']';
	}
};

template<typename values>
std::ostream& print_values(std::ostream& os)
{
	return os << value_printer<values>();
}

#endif // !VALUELIST_H
