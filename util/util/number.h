#ifndef NUMBER_H
#define NUMBER_H

#define NUMBER_SCALE (1 + 0xFFFFFF) // 24 bits of decimals

// Saves raw number in fixed point format
template<long long number>
struct number_c
{
	enum {raw_value = number};

	double get() const { return raw_value * 1.0 / NUMBER_SCALE; }
};

// save a number in type
#define NUMBER_MAKE(numb) number_c<static_cast<long long>((numb) * NUMBER_SCALE)>
// compile time get a number from the value
#define NUMBER_GET_RAW(raw_value) (raw_value * 1.0 / NUMBER_SCALE)
// compile time get a number from the type
#define NUMBER_GET_TYPE(typed_number) NUMBER_GET_RAW(typed_number::raw_value)


#endif // !NUMBER_H
