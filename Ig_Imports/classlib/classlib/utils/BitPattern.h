#ifndef CLASSLIB_BIT_PATTERN_H
# define CLASSLIB_BIT_PATTERN_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// This should all be partial template specialisation with <Pat,Width,[01]>,
// but MSVC++ doesn't grok it.  So make a wrapper with the parameter we want
// to specialise (N), and specialise on that.
template <unsigned int N>
struct BitPatternHelp
{
    template <class T, unsigned int Pat, unsigned int Width>
    struct PatWrapper
    {
	enum { Pattern = (((T) BitPatternHelp<N/2+N%2>::template PatWrapper<T,Pat,Width>::Pattern
			   << (T) (Width*(N/2)))
			  | (T) BitPatternHelp<N/2>::template PatWrapper<T,Pat,Width>::Pattern) };
    };
};

/** Trivial specialisation for null-width pattern.  */
template <>
struct BitPatternHelp<0>
{
    template <class T, unsigned int Pat, unsigned int Width>
    struct PatWrapper
    {
	enum { Pattern = (T) 0 };
    };
};

/** Trivial specialisation for single-width pattern.  */
template <>
struct BitPatternHelp<1>
{
    template <class T, unsigned int Pat, unsigned int Width>
    struct PatWrapper
    {
	enum { Pattern = (T) Pat };
    };
};

/** Construct a repetitive bit pattern.

    Builds up a bit pattern be repeating the basic pattern @a Pat of
    @c Width bits wide to be @a N bits wide; @a N must be an integral
    multiple of @a Width.  The building is done by progressively
    smaller patterns with template specialisations to terminate the
    trivial cases.  */
template <class T, unsigned int Pat, unsigned int Width, unsigned int N>
struct BitPattern
{
    enum { Pattern = BitPatternHelp<N>::template PatWrapper<T,Pat,Width>::Pattern };
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_BIT_PATTERN_H
