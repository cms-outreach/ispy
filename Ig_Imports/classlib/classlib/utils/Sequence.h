#ifndef CLASSLIB_SEQUENCE_H
# define CLASSLIB_SEQUENCE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/Range.h"
# include "classlib/utils/IntTraits.h"
# include "classlib/utils/DebugAids.h"
# include "classlib/utils/Log.h"
# include <stdexcept>
# include <typeinfo>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

template <class T> class Sequence;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

template <class T>
logstream &operator<< (logstream &log, const Sequence<T> &s);

//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Sequence generator.

    Each invocation of a #Sequence generates a value in a range.  The
    sequence starts at the lowest value and after that ever increasing
    values are produced.  When the upper limit has been reached, the
    behaviour depends on the #wraps flag.  */
template <class T>
class Sequence
{
public:
    Sequence (bool wraps = false);
    Sequence (T low, T high, bool wraps = false);
    Sequence (const Range<T> &limits, bool wraps = false);
    // Implicit copy constructor.
    // Implicit destructor.
    // Implicit assignment operator.

    Range<T>		limits (void) const;
    void		limits (const Range<T> &value);

    bool		wraps (void) const;
    void		wraps (bool value);
    bool		wrapped (void);
    
    T			next (void);
    T			current (void) const;
    void		reset (void);

    friend logstream &	operator<< <> (logstream &log, const Sequence &s);

private:
    Range<T>		m_limits;  /*< The range of possible values. */
    T			m_current; /*< The current sequence value. */
    bool		m_wraps;   /*< Flag indicating the wrapping mode. */
    bool		m_wrapped; /*< Flag indicating if wrapping has
				       happened.  Reading the flag
				       clears it. */
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

template <class T>
inline logstream &
operator<< (logstream &log, const Sequence<T> &s)
{
#ifndef NLOG
    ASSERT (s.m_limits.low () < s.m_limits.high ());
    ASSERT (s.m_current >= s.m_limits.low ());
    ASSERT (s.m_current <= s.m_limits.high ());

    return log
	<< "Sequence<" << typeid(T).name () << "> @ " << &s << " {\n" << indent
	<< "limits  [" << s.m_limits.low ()
	<< ", " << s.m_limits.high () << "]\n"
	<< "current " << s.m_current << endl
	<< "wraps   " << s.m_wraps << endl
	<< "wrapped " << s.m_wrapped << endl
	<< undent << "}";
#endif // !NLOG
}

//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Initialize a Sequence.

    If no limits are given, the entire range of @c T is used.  The
    current value is set to the low limit; the next call to #next()
    will return one higher than that.

    @param wraps	Flag indicating whether the sequence should
			wrap when the upper limit has been reached and
			the #next() value is requested.  If the flag
			is @c false, overflow throws an exception.
			Otherwise the sequence wraps back to the low
			limit.  */
template <class T>
inline
Sequence<T>::Sequence (bool wraps /* = false */)
    : m_limits (IntTraits<T>::Min, IntTraits<T>::Max),
      m_current (IntTraits<T>::Min),
      m_wraps (wraps),
      m_wrapped (false)
{
    ASSERT (m_limits.low () < m_limits.high ());
    ASSERT (m_current >= m_limits.low ());
    ASSERT (m_current <= m_limits.high ());
}

/** @param low, high	The range of sequence values.  */
template <class T>
inline
Sequence<T>::Sequence (T low, T high, bool wraps /* = false */)
    : m_limits (low, high),
      m_current (low),
      m_wraps (wraps),
      m_wrapped (false)
{
    ASSERT (m_limits.low () < m_limits.high ());
    ASSERT (m_current >= m_limits.low ());
    ASSERT (m_current <= m_limits.high ());
}

/** @param limits	The range of sequence values.  */
template <class T>
inline
Sequence<T>::Sequence (const Range<T> &limits, bool wraps /* = false */)
    : m_limits (limits),
      m_current (limits.low ()),
      m_wraps (wraps),
      m_wrapped (false)
{
    ASSERT (m_limits.low () < m_limits.high ());
    ASSERT (m_current >= m_limits.low ());
    ASSERT (m_current <= m_limits.high ());
}

/** Return the current sequence value range.  */
template <class T>
inline Range<T>
Sequence<T>::limits (void) const
{ return m_limits; }

/** Set the sequence value range to @a value.  If the current value
    does not fall into this range, it is corrected to be either the
    low or the high limit, whichever is closer.  If the current value
    is modified, the #wrapped() flag is set.  No #std::range_error
    exception is thrown.  */
template <class T>
inline void
Sequence<T>::limits (const Range<T> &value)
{
    ASSERT (m_limits.low () < m_limits.high ());
    ASSERT (m_current >= m_limits.low ());
    ASSERT (m_current <= m_limits.high ());
    ASSERT (value.low () < value.high ());

    m_limits = limits;
    if (m_current < m_limits.low ())
    {
	m_wrapped = true;
	m_current = m_limits.low ();
    }
    else if (m_current > m_limits.high ())
    {
	m_wrapped = true;
    	m_current = m_limits.high ();
    }
}

/** Return the current wrap mode.  If @c true, the value next from the
    upper limit rolls back to the low limit.  */
template <class T>
inline bool
Sequence<T>::wraps (void) const
{ return m_wraps; }

/** Set the current wrap mode to @a value.  If it is @c true, the
    value next from the upper limit rolls back to the low limit.  */
template <class T>
inline void Sequence<T>::wraps (bool value)
{ m_wraps = value; }

/** Check if the sequence has wrapped.  The flag is automatically
    cleared as a side effect.  */
template <class T>
inline bool
Sequence<T>::wrapped (void)
{ bool wrapped = m_wrapped; m_wrapped = false; return wrapped; }

/** Generate the next sequence value.

    Wrapping is considered if the current value already equals to the
    upper limit.  If wrapping mode is on, the value is reset to the
    low limit of the range, and #wrapped() is set to @c true;
    otherwise #std::range_error is thrown.

    Subsequent calls to #current() will return the same value.  */
template <class T>
inline T
Sequence<T>::next (void)
{
    ASSERT (m_limits.low () < m_limits.high ());
    ASSERT (m_current >= m_limits.low ());
    ASSERT (m_current <= m_limits.high ());

    if (m_current == m_limits.high ())
	if (m_wraps)
	{
	    m_wrapped = true;
	    m_current = m_limits.low ();
	}
	else
	    throw std::range_error ("Sequence upper limit reached");
    else
	++m_current;

    ASSERT (m_current >= m_limits.low ());
    ASSERT (m_current <= m_limits.high ());
    return m_current;
}

/** Get the current sequence value.  */
template <class T>
inline T
Sequence<T>::current (void) const
{
    ASSERT (m_limits.low () < m_limits.high ());
    ASSERT (m_current >= m_limits.low ());
    ASSERT (m_current <= m_limits.high ());
    return m_current;
}

/** Resets the current value to the low limit and clears the
    #wrapped() flag.  */
template <class T>
inline void
Sequence<T>::reset (void)
{
    ASSERT (m_limits.low () < m_limits.high ());
    ASSERT (m_current >= m_limits.low ());
    ASSERT (m_current <= m_limits.high ());

    m_current = m_limits.low ();
    m_wrapped = false;

    ASSERT (m_limits.low () < m_limits.high ());
    ASSERT (m_current >= m_limits.low ());
    ASSERT (m_current <= m_limits.high ());
}

} // namespace lat
#endif // CLASSLIB_SEQUENCE_H
