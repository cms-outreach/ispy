#ifndef CLASSLIB_STRING_HACK_H
# define CLASSLIB_STRING_HACK_H

//<<<<<< INCLUDES                                                       >>>>>>
//<<<<<< PUBLIC DEFINES                                                 >>>>>>

// FIXME: Temporary hack until we are fully migrated
#if defined __GNUC__ && __GNUC__ <= 2
# define STRCOMPARE(s,other,pos,len)	s.compare (other, pos, len)
#else
# define STRCOMPARE(s,other,pos,len)	s.compare (pos, len, other)
#endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_STRING_HACK_H
