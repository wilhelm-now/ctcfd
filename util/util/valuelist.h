#ifndef VALUELIST_H
#define VALUELIST_H

// utility functions for typelists of values
#include <util/typelist.h>

#include <ostream>

template<typename values>
struct value_printer;

template<typename T, typename U>
struct value_printer<typelist<T, U> >
{
	friend std::ostream& operator<<(std::ostream& os, value_printer)
	{
		return os << T().get() << ", " << value_printer<U>();
	}
};

template<typename T>
struct value_printer<typelist<T, null_t> >
{
	friend std::ostream& operator<<(std::ostream& os, value_printer)
	{
		return os << T().get();
	}
};

namespace detail
{
	template<typename values>
	struct value_printer2d_impl;

	template<typename Head, typename Tail>
	struct value_printer2d_impl<typelist<Head, Tail> >
	{
		friend std::ostream& operator<<(std::ostream& os, value_printer2d_impl)
		{
			return os << '[' << value_printer<Head>() << "],\n" 
				<< value_printer2d_impl<Tail>();
		}
	};

	template<typename Head>
	struct value_printer2d_impl<typelist<Head, null_t> >
	{
		friend std::ostream& operator<<(std::ostream& os, value_printer2d_impl)
		{
			return os << '[' << value_printer<Head>() << ']';
		}
	};
}

template<typename values>
struct value_printer2d
{
	friend std::ostream& operator<<(std::ostream& os, value_printer2d)
	{
		return os << '[' << detail::value_printer2d_impl<values>() << ']';
	}
};


template<typename values>
std::ostream& print_values(std::ostream& os)
{
	return os << value_printer<values>();
}

#endif // !VALUELIST_H
