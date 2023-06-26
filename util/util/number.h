#ifndef NUMBER_H
#define NUMBER_H

// Saves raw number
template<long long number>
struct number_t
{
	enum {value = number};
};



#endif // !NUMBER_H
