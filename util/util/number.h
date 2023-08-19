#ifndef NUMBER_H
#define NUMBER_H

#include <ostream>

#define NUMBER_SCALE (0x1000000ll) // 24 bits of decimals

typedef long long number_rep;

// Saves raw number in fixed point format
template<number_rep number>
struct number_c
{
	enum {raw_value = number};

	double get() const { return raw_value * 1.0 / NUMBER_SCALE; }
	
};

template<number_rep raw>
std::ostream& operator<<(std::ostream& os, number_c<raw> number)
{
	return os << number.get();
}


// save a number in type
#define NUMBER_MAKE(numb) number_c<static_cast<number_rep>((numb) * NUMBER_SCALE)>
// compile time get a number from the value
#define NUMBER_GET_RAW(raw_value) (raw_value * 1.0 / NUMBER_SCALE)
// compile time get a number from the type
#define NUMBER_GET_TYPE(typed_number) NUMBER_GET_RAW(typed_number::raw_value)


#endif // !NUMBER_H
