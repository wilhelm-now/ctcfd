# CTCFD
This repository is an attempt at compile-time computational fluid dynamics. Its goal is to reach lid-driven cavity flow and follows the well known [12 steps to Navier-Stokes](https://github.com/barbagroup/CFDPython) project stopping at step 11. It is implemented using types and typelists with lot's of inspiration from Alexandrescu's [Modern C++ Design](https://en.wikipedia.org/wiki/Modern_C%2B%2B_Design). GCC accepts it as C++98 code, Clang does not.


# Why
I thought it would be fun.

# How

## Numbers
Numbers are saved in types
```c++
typedef long long representation_type;

template<representation_type number>
struct number_constant
{
    enum {value = number};
};
```

To save floating point numbers we multiply them by some big number first. We can divide by this big number when retrieving the number. GCC allows static_cast'ing from double at compile time, Clang does not.

```c++
#define BIG_NUMBER 123456789

#define NUMBER_MAKE(number) number_constant<static_cast<representation_type>((number) * BIG_NUMBER)>

#define NUMBER_GET(number_type) (number_type::value * 1.0/ BIG_NUMBER)
```

See this in util/number.h.

## Typelists
Alexanderscu's introduction to typelists is better than anything I can write here. The usual definition is used.

```c++
struct null_t {}; // Tag type indicating end of typelist

template<typename T, typename U>
struct typelist
{
    typedef T head;
    typedef U tail;
};
```


A typelist holding a single type `T1` is has type `typelist<T1, null_t>`. A typelist holding two types `T1` and `T2` has type `typelist<T1, typelist<T2, null_t> >`. To hold many numbers the previous `number_constant` with helper macro is used.

```c++
typedef typelist<NUMBER_MAKE(0), typelist<NUMBER_MAKE(1), typelist<NUMBER_MAKE(2), null_t> > > numbers;
```

For development macros can help define longer typelists.

```c++
#define TYPELIST_1(T1) typelist<T1, null_t>
#define TYPELIST_2(T1, T2) typelist<T1, TYPELIST_1(T2)>
#define TYPELIST_3(T1, T2, T3) typelist<T1, TYPELIST_2(T2, T3)>
// ... and so on to TYPELIST_N
```
Variations of this are used for many typenames, many numbers and typelists where the last named type is not `null_t` and could be another typelist.

## Computation

Computation is done recursively. First the front elements are used to do one step of computation. Then the result is stored at the head of a new typelist and the tail of the new typelist is found from applying the same function to the tail of the input typelist. 

Step3 - Diffusion is a useful example. 

```c++
// After discretization and rearranging next guess for value = u_current_center + (nu*dt/(dx*dx))*(u_current_right – 2*u_current_center + u_current_left)
template<typename U1, typename U2, typename U3, typename tail>
class compute_diffusion<typelist<U1, typelist<U2, typelist<U3, tail> > > >
{
    typedef NUMBER_MAKE(NUMBER_GET(U2) + (NU*DT/(DX*DX))*(NUMBER_GET(U3) - 2*NUMBER_GET(U2) + NUMBER_GET(U1))) computed;
public:
    typedef typelist<
        computed, // computed at this step
        typename compute_diffusion<typelist<U2, typelist<U3, tail> > >::type // recursively computed at next step
        > type;
};
```

As operations become more complex they are moved to their own metafunction. In the above example this could be written like below.

```c++
template<typename U1, typename U2, typename U3>
struct compute_diffusion_point
{
    typedef NUMBER_MAKE(NUMBER_GET(U2) + (NU*DT/(DX*DX))*(NUMBER_GET(U3) - 2*NUMBER_GET(U2) + NUMBER_GET(U1))) type;
};
```

### Boundary conditions

#### Far edge

Recursion is stopped when the typelist reaches the end. This is also when boundary conditions at the far edge is applied by specializing the computation struct. Fixed edge values are simple.

```c++
template<typename U1, typename U2>
class compute_diffusion<typelist<U1, typelist<U2, null_t> > >
{
public:
    typedef typelist<NUMBER_MAKE(KNOWN_FAR_EDGE_VALUE), null_t> type; // when the edge value is specified
};
```

For fixed gradient boundary conditions the recursion stops one step earlier and sets the edge value depending on the previous value. In this project only zero gradient is used and then the value is copied.

```c++
template<typename U1, typename U2, typename U3>
class compute_diffusion<typelist<U1, typelist<U2, typelist<U3, null_t> > > >
{
    typedef typename compute_diffusion_point<U1, U2, U3>::type computed;
public:
    typedef typelist<computed, typelist<computed, null_t> > type; // repeat computed to set zero gradient
};
```

#### Near edge

In the recursive computation there is no way to tell if the current iteration is the first. Instead another metafunction is used to initiate the computation for the rest of the domain and that result is saved in a new typelist with first element dependent on the boundary condition. Fixed value boundary conditions are simple.

```c++
template<typename types>
class diffusion
{
public:
    typedef typelist<NUMBER_MAKE(KNOWN_NEAR_EDGE_VALUE), typename compute_diffusion<types>::type> type;
};
```

Again, fixed gradient sets the edge value of the output dependent on the inner value. Zero gradient boundary condition is shown below.

```c++
template<typename types>
class diffusion
{
    typedef typename compute_diffusion<types>::type computed;
public:
    typedef typelist<typename computed::head, computed> type;
};
```

### Initial conditions

Initial conditions are computed from the index of each node. A type `for_i` is specialized with a type holding a nested template which works on indicies to create values. This is in the style of [Boost MPL wrapped metafunctions](https://www.boost.org/doc/libs/1_84_0/libs/mpl/doc/tutorial/implementing.html) but the inner struct is called `func` instead of `apply`. Using the inner struct allows the outer type to be a template as well and this is useful for binding function arguments. The two dimensional version `for_ij` uses this technique. Both `for_i` and `for_ij` are defined in util/for_i.h

Constants like `DX` are defined in a header configured depending on build settings. Below is an example of a square wave used for steps 1 through 4.

```c++
struct initial
{
	template<unsigned idx>
	struct func
	{
		typedef NUMBER_MAKE((idx* DX >= 0.5 && idx * DX <= 1.0) ? 2.0 : 1.0) type;
	};
};
```

Complicated initial conditions for Step4 are generated using python.


### Time iteration

Iteration in time is also done recursively. The base case is defined by initial conditions and subsequent iterations do some transform on the previous value. A possible implementation of diffusion is shown below.

```c++

template<unsigned iteration>
struct diffusion;

// Base case with initial conditions
template<>
struct diffusion<0>
{
    typedef typename for_i<NX, initial>::type type;
};

template<unsigned iteration>
struct diffusion
{
    typedef typelist<
        typename diffusion<iteration - 1>::type::head, // fixed value near boundary condition
        typename compute_diffusion<typename diffusion<iteration - 1>::type>::type
        > type;
};

```
