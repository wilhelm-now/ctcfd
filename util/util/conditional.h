#ifndef CONDITIONAL_H
#define CONDITIONAL_H

template<bool condition, typename true_type, typename false_type>
struct conditional
{
	typedef typename true_type type;
};

// false specialization
template<typename true_type, typename false_type>
struct conditional<false, true_type, false_type>
{
	typedef typename false_type type;
};

#endif // !CONDITIONAL_H
