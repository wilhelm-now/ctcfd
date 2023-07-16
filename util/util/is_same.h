#ifndef IS_SAME_H
#define IS_SAME_H

template<typename T, typename U>
struct is_same
{
	enum {value = false};
};

template<typename T>
struct is_same<T, T>
{
	enum {value = true};
};

#endif // !IS_SAME_H
