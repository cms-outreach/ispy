//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/StringFormat.h"
#include "classlib/utils/DebugAids.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <limits.h>
#include <math.h>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

std::ostream &operator<< (std::ostream &out, const StringFormat &x)
{ const std::string &xx = x; return out << xx; }

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Construct a format from @a pattern.  */
StringFormat::StringFormat (const std::string &pattern)
    : m_value (pattern)
{ dorescan (); }

/** Private constructor used by #rescan(). */
StringFormat::StringFormat (void)
{}

/** Private constructor used by #doarg() to build a new format pattern
    after a replacement.  Copies @a value and [@a begin, @a end) range
    but replaces @a replacement into @a value at @a offset for length
    @a len.  Remaining arguments in the [@a begin, @a end) range are
    adjusted to take the offset shift into account.  (FIXME: This is
    horribly inefficient.  Think again a better implementation.) */
StringFormat::StringFormat (const std::string &value,
			    const std::string &replacement,
			    size_t offset, size_t len,
			    ArgList::const_iterator begin,
			    ArgList::const_iterator end)
    : m_value (value)
{
    m_value.replace (offset, len, replacement);

    for ( ; begin != end; ++begin)
	if (begin->first < offset)
	    m_args.push_back (*begin);
	else
	    m_args.push_back (std::make_pair (begin->first - len
					      + replacement.size (),
					      begin->second));
}

/** Return the result from replacements so far.  */
StringFormat::operator const std::string & (void) const
{ return m_value; }

/** Return the result from replacements so far.  */
const std::string &
StringFormat::value (void) const
{ return m_value; }

//////////////////////////////////////////////////////////////////////
/** Return a string padded to @a fieldwidth from data @a buf of length
    @a len.  @a fieldwidth gives the minimum width of the field; if
    the data is longer than that, it is returned as such.  A negative
    @a fieldwidth specifies @a buf to be padded to left instead of the
    default right.  */
inline std::string
StringFormat::pad (const char *buf, size_t len, int fieldwidth)
{
    std::string result;

    if (fieldwidth > 0 && len < (size_t) fieldwidth)
	result.append (fieldwidth - len, ' ');

    result.append (buf, len);

    if (fieldwidth < 0 && len < (size_t) -fieldwidth)
	result.append (-fieldwidth - len, ' ');

    return result;
}

//////////////////////////////////////////////////////////////////////
/** Convert @a val into the buffer ending in @a next in @a base.
    Returns pointer to the last character inserted into the buffer.
    Thus the formatted string starts at the return value and ends one
    byte before the @a next that was passed in.  Thus @a next is not
    written into; the byte before it is.  @a base must be at least 2
    and no greater than 36.  There must be enough space in the buffer
    to produce the value.  */
inline char *
StringFormat::format (char *next, unsigned long val, int base)
{
    // FIXME: Options? Pad-with-zero/-space, upper-/lowercase

    ASSERT (base >= 2);
    ASSERT (base <= 36);

    // Digits for conversion in bases 2 -- 36.
    static const char digits [] = "0123456789abcdefghijklmopqrstuvwxyz";

    // Produce value
    if (val == 0)
	*--next = '0';
    else
	while (val)
	{
	    *--next = digits [val % base];
	    val /= base;
	}

    return next;
}

/** Format @a value in @a base into a string at least @a fieldwidth
    characters wide.  The absolute value of @a fieldwidth specifies
    the minimum length of the resulting string, including a possible
    leading minus sign.  If the formatted number is longer than the
    requested width, it is returned as such.  Otherwise it is padded
    with spaces on right if @a fieldwidth is positive, or left if @a
    fieldwidth is negative.  The conversion @a base must be at least 2
    and no greater than 36.  */
std::string
StringFormat::format (long val, int fieldwidth, int base)
{
    // Maximum number of digits that a conversion can produce for a long.
    // The longest sequence can be in base-2 (= binary).
    static const int MAXDIGITS = sizeof (val) * CHAR_BIT + 1;

    // Conversion buffer.  We convert from end to front, so stick null
    // there, then the value, and finally possibly the sign.
    char buf [MAXDIGITS + 2];
    char *next = buf + sizeof (buf);

    *--next = 0;
    next = format (next, abs (val), base);
    if (val < 0)
	*--next = '-';

    ASSERT (next < buf + sizeof (buf) - 1);
    ASSERT (next >= buf);

    // Pad value to desired width
    return pad (next, buf + sizeof (buf) - 1 - next, fieldwidth);
}

/** Format @a value in @a base into a string at least @a fieldwidth
    characters wide.  The absolute value of @a fieldwidth specifies
    the minimum length of the resulting string, including a possible
    leading minus sign.  If the formatted number is longer than the
    requested width, it is returned as such.  Otherwise it is padded
    with spaces on right if @a fieldwidth is positive, or left if @a
    fieldwidth is negative.  The conversion @a base must be at least 2
    and no greater than 36.  */
std::string
StringFormat::format (unsigned long val, int fieldwidth, int base)
{
    // Maximum number of digits that a conversion can produce for a long.
    // The longest sequence can be in base-2 (= binary).
    static const int MAXDIGITS = sizeof (val) * CHAR_BIT + 1;

    // Conversion buffer.  We convert from end to front, so stick null
    // there, then the value.
    char buf [MAXDIGITS + 2];
    char *next = buf + sizeof (buf);

    *--next = 0;
    next = format (next, val, base);

    ASSERT (next < buf + sizeof (buf) - 1);
    ASSERT (next >= buf);

    // Pad value to desired width
    return pad (next, buf + sizeof (buf) - 1 - next, fieldwidth);
}

/** Format @a value into a string at least @a fieldwidth characters
    wide with floating point format @a fmt and precision @a precision.
    The arguments for @a fmt, @a fieldwidth and @a precision are as
    those for C @c sprintf() function.  This is equivalent to the
    rules obeyed by the other overloaded methods except for the added
    complexity of the interaction of the different floating point
    formats, @a fieldwidth and @a precision.  */
std::string
StringFormat::format (double val, int fieldwidth, char fmt, int precision)
{
    // Allocate a conversion buffer plus space for null.  Note that
    // binary to decimal conversion is always fully precise because
    // divisors of powers of two never result in infinite decimal
    // sequences.  As the lower limit, make a conservative estimate by
    // assuming that every bit in the value could be contributing
    // digits.  Allocate extra space for the possible leading sign,
    // decimal point, exponent indicator and exponent sign.  Thus a
    // 64-bit double may produce 20+4 decimal representation.  The
    // conversions for "+/-Inf" and "NaN" are expected to be shorter
    // than this.
    // 
    // Add on top of this conservative estimate any extra space the
    // caller requests.  For example, caller may set "fieldwidth" or
    // "precision" to values larger than necessary for the number
    // itself.  Simply add their absolute values to our lower limit,
    // that gives us plenty of room for various extra additions
    // without making too complicated calculation on just how much
    // space is needed.
    //
    // And of course what we really should do here is use snprintf()
    // instead of all these magic calculations.

    ASSERT (fmt=='e' || fmt=='E' || fmt=='f' || fmt=='g' || fmt=='G');

    static const int MAXNATURAL // 30103 =~ M_LN2 / M_LN10 * 100000
	= (sizeof (val) * CHAR_BIT * 30103) / 100000 + 1 + 4 + 1;
    int		maxsize = MAXNATURAL + abs (fieldwidth) + abs (precision);
    std::string	buffer (maxsize, '\0');
    char	fmtstr [] = "%*.*f";
    size_t	len;

    fmtstr [sizeof (fmtstr) - 2] = fmt;
    len = sprintf ((char *) buffer.data(), fmtstr, fieldwidth, precision, val);
    ASSERT (len < buffer.size ());
    buffer.resize (len);

    return buffer;
}

//////////////////////////////////////////////////////////////////////
/** Collect all parameters in the pattern.  */
StringFormat
StringFormat::rescan (void) const
{
    StringFormat result (*this);
    result.dorescan ();
    return result;
}

/** Collect all parameters in the pattern.  */
void
StringFormat::dorescan (void)
{
    // Scan for references in value.  We need to work on a copy since
    // we need to overwrite the referenes.  (FIXME: Use an algorithm
    // that finds all values and inserts them into the list in order
    // as they are found.)
    std::string	tmp (m_value);
    size_t	offset = 0;
    size_t	len = 0;

    while (nextarg (tmp, offset, len))
    {
	// Record the position at the back of the list.  This puts the
	// first element to replace in the front, last at the back.
	m_args.push_back (std::make_pair (offset, len));

	// Overwrite the marker so we won't pick it up the next time.
	tmp [offset] = 'X';
    }
}

/** Find the next smallest argument reference from string @a in.  The
    location of the reference will be put into @a offset and the
    length of the string to be replaced will be put into @a len and
    the function returns @c true.  If no argument reference is found,
    @a offset and @a len are not modified and the return value is @c
    false.  */
bool
StringFormat::nextarg (const std::string &in, size_t &offset, size_t &len)
{
    const char	digits [] = "0123456789";
    const char	*begin = in.data ();
    const char	*end = begin + in.size ();
    const char	*d;
    int		arg = -1;

    for (const char *p = begin; p != end; ++p)
	// Ignore characters not our marker and a trailing '%'
	if (*p == '%' && p+1 != end && (d = strchr (digits, *++p)))
	{
	    // "%[digit]".  Keep going as long as we see digits and
	    // build argument reference number from them.
	    const char	*start = p;
	    int		n = d - digits;

	    while (p != end && (d = strchr (digits, *++p)))
		n = n * 10 + (d - digits);

	    // Grab this reference if it is the smallest we've seen.
	    if (arg < 0 || n < arg)
	    {
		arg = n;
		offset = start - 1 - begin;
		len = p - start + 1;
	    }

	    // Backtrack one place, we are already at the chacter we
	    // should reconsider -- the loop above will increment p.
	    --p;
	}

    return arg >= 0;
}


//////////////////////////////////////////////////////////////////////
/** Actually replace next smallest argument reference.  */
StringFormat
StringFormat::doarg (const std::string &replacement) const
{
    ArgList::const_iterator item = m_args.begin ();
    ArgList::const_iterator last = m_args.end ();
    size_t offset = m_value.size ();
    size_t len = 0;

    if (item != last)
    {
	offset = item->first;
	len = item->second;
	++item;
    }

    return StringFormat (m_value, replacement, offset, len, item, last);
}


/** Replace the next smallest argument reference of the form "%N" in
    the string format with the conversion of @a val to a string of at
    least @a fieldwidth wide in @a base.

    The argument references are of the form "%N" where the N is an
    arbitrary number.  At each #arg() call the lowest integer N is
    replaced with the formatted value.  If no more references are left
    in the pattern, the formatted value is appended to the string, but
    this usage is not adviced.  The format pattern is scanned for
    argument references only when it is constructed or if #rescan() is
    called; this makes it safe to replace values that might otherwise
    result in further argument references.

    The %-characters in the format pattern cannot be escaped and the
    interpretation of the digits following it cannot be stopped.  Both
    situations are easily handled with an extra replacement pattern:
    StringFormat("%1%2").arg(10).arg('%') produces "10%" and
    StringFormat("%1%2").arg(10).arg("00") produces "1000".

    The absolute value of @a fieldwidth specifies the minimum length
    of the formatted replacement string, including a possible leading
    minus sign.  If the formatted value is longer than the requested
    width, it is replaced as such.  Otherwise it is padded with spaces
    on right if @a fieldwidth is positive, or left if @a fieldwidth is
    negative.  The conversion @a base must be at least 2 and no
    greater than 36.

    Returns a new #StringFormat with the lowest argument reference
    replaced.  */
StringFormat
StringFormat::arg (long			val,
		   int			fieldwidth /* = 0 */,
		   int			base /* = 10 */) const
{ return doarg (format (val, fieldwidth, base)); }

/** Replace the next smallest argument reference of the form "%N" in
    the string format with the conversion of @a val to a string of at
    least @a fieldwidth wide in @a base.

    The argument references are of the form "%N" where the N is an
    arbitrary number.  At each #arg() call the lowest integer N is
    replaced with the formatted value.  If no more references are left
    in the pattern, the formatted value is appended to the string,
    but this usage is not adviced.  The format pattern is scanned for
    argument references only when it is constructed or if #rescan() is
    called; this makes it safe to replace values that might otherwise
    result in further argument references.

    The %-characters in the format pattern cannot be escaped and the
    interpretation of the digits following it cannot be stopped.  Both
    situations are easily handled with an extra replacement pattern:
    StringFormat("%1%2").arg(10).arg('%') produces "10%" and
    StringFormat("%1%2").arg(10).arg("00") produces "1000".

    The absolute value of @a fieldwidth specifies the minimum length
    of the formatted replacement string, including a possible leading
    minus sign.  If the formatted value is longer than the requested
    width, it is replaced as such.  Otherwise it is padded with spaces
    on right if @a fieldwidth is positive, or left if @a fieldwidth is
    negative.  The conversion @a base must be at least 2 and no
    greater than 36.

    Returns a new #StringFormat with the lowest argument reference
    replaced.  */
StringFormat
StringFormat::arg (unsigned long	val,
		   int			fieldwidth /* = 0 */,
		   int			base /* = 10 */) const
{ return doarg (format (val, fieldwidth, base)); }

/** Replace the next smallest argument reference of the form "%N" in
    the string format with the conversion of @a val to a string of at
    least @a fieldwidth wide in @a base.

    The argument references are of the form "%N" where the N is an
    arbitrary number.  At each #arg() call the lowest integer N is
    replaced with the formatted value.  If no more references are left
    in the pattern, the formatted value is appended to the string,
    but this usage is not adviced.  The format pattern is scanned for
    argument references only when it is constructed or if #rescan() is
    called; this makes it safe to replace values that might otherwise
    result in further argument references.

    The %-characters in the format pattern cannot be escaped and the
    interpretation of the digits following it cannot be stopped.  Both
    situations are easily handled with an extra replacement pattern:
    StringFormat("%1%2").arg(10).arg('%') produces "10%" and
    StringFormat("%1%2").arg(10).arg("00") produces "1000".

    The absolute value of @a fieldwidth specifies the minimum length
    of the formatted replacement string, including a possible leading
    minus sign.  If the formatted value is longer than the requested
    width, it is replaced as such.  Otherwise it is padded with spaces
    on right if @a fieldwidth is positive, or left if @a fieldwidth is
    negative.  The conversion @a base must be at least 2 and no
    greater than 36.

    Returns a new #StringFormat with the lowest argument reference
    replaced.  */
StringFormat
StringFormat::arg (int			val,
		   int			fieldwidth /* = 0 */,
		   int			base /* = 10 */) const
{ return doarg (format ((long) val, fieldwidth, base)); }

/** Replace the next smallest argument reference of the form "%N" in
    the string format with the conversion of @a val to a string of at
    least @a fieldwidth wide in @a base.

    The argument references are of the form "%N" where the N is an
    arbitrary number.  At each #arg() call the lowest integer N is
    replaced with the formatted value.  If no more references are left
    in the pattern, the formatted value is appended to the string,
    but this usage is not adviced.  The format pattern is scanned for
    argument references only when it is constructed or if #rescan() is
    called; this makes it safe to replace values that might otherwise
    result in further argument references.

    The %-characters in the format pattern cannot be escaped and the
    interpretation of the digits following it cannot be stopped.  Both
    situations are easily handled with an extra replacement pattern:
    StringFormat("%1%2").arg(10).arg('%') produces "10%" and
    StringFormat("%1%2").arg(10).arg("00") produces "1000".

    The absolute value of @a fieldwidth specifies the minimum length
    of the formatted replacement string, including a possible leading
    minus sign.  If the formatted value is longer than the requested
    width, it is replaced as such.  Otherwise it is padded with spaces
    on right if @a fieldwidth is positive, or left if @a fieldwidth is
    negative.  The conversion @a base must be at least 2 and no
    greater than 36.

    Returns a new #StringFormat with the lowest argument reference
    replaced.  */
StringFormat
StringFormat::arg (unsigned int		val,
		   int			fieldwidth /* = 0 */,
		   int			base /* = 10 */) const
{ return doarg (format ((unsigned long) val, fieldwidth, base)); }

/** Replace the next smallest argument reference of the form "%N" in
    the string format with the conversion of @a val to a string of at
    least @a fieldwidth wide in @a base.

    The argument references are of the form "%N" where the N is an
    arbitrary number.  At each #arg() call the lowest integer N is
    replaced with the formatted value.  If no more references are left
    in the pattern, the formatted value is appended to the string,
    but this usage is not adviced.  The format pattern is scanned for
    argument references only when it is constructed or if #rescan() is
    called; this makes it safe to replace values that might otherwise
    result in further argument references.

    The %-characters in the format pattern cannot be escaped and the
    interpretation of the digits following it cannot be stopped.  Both
    situations are easily handled with an extra replacement pattern:
    StringFormat("%1%2").arg(10).arg('%') produces "10%" and
    StringFormat("%1%2").arg(10).arg("00") produces "1000".

    The absolute value of @a fieldwidth specifies the minimum length
    of the formatted replacement string, including a possible leading
    minus sign.  If the formatted value is longer than the requested
    width, it is replaced as such.  Otherwise it is padded with spaces
    on right if @a fieldwidth is positive, or left if @a fieldwidth is
    negative.  The conversion @a base must be at least 2 and no
    greater than 36.

    Returns a new #StringFormat with the lowest argument reference
    replaced.  */
StringFormat
StringFormat::arg (short		val,
		   int			fieldwidth /* = 0 */,
		   int			base /* = 10 */) const
{ return doarg (format ((long) val, fieldwidth, base)); }

/** Replace the next smallest argument reference of the form "%N" in
    the string format with the conversion of @a val to a string of at
    least @a fieldwidth wide in @a base.

    The argument references are of the form "%N" where the N is an
    arbitrary number.  At each #arg() call the lowest integer N is
    replaced with the formatted value.  If no more references are left
    in the pattern, the formatted value is appended to the string,
    but this usage is not adviced.  The format pattern is scanned for
    argument references only when it is constructed or if #rescan() is
    called; this makes it safe to replace values that might otherwise
    result in further argument references.

    The %-characters in the format pattern cannot be escaped and the
    interpretation of the digits following it cannot be stopped.  Both
    situations are easily handled with an extra replacement pattern:
    StringFormat("%1%2").arg(10).arg('%') produces "10%" and
    StringFormat("%1%2").arg(10).arg("00") produces "1000".

    The absolute value of @a fieldwidth specifies the minimum length
    of the formatted replacement string, including a possible leading
    minus sign.  If the formatted value is longer than the requested
    width, it is replaced as such.  Otherwise it is padded with spaces
    on right if @a fieldwidth is positive, or left if @a fieldwidth is
    negative.  The conversion @a base must be at least 2 and no
    greater than 36.

    Returns a new #StringFormat with the lowest argument reference
    replaced.  */
StringFormat
StringFormat::arg (unsigned short	val,
		   int			fieldwidth /* = 0 */,
		   int			base /* = 10 */) const
{ return doarg (format ((unsigned long) val, fieldwidth, base)); }

/** Replace the next smallest argument reference of the form "%N" in
    the string format with the character @a val padded to a string of
    at least @a fieldwidth wide.

    The argument references are of the form "%N" where the N is an
    arbitrary number.  At each #arg() call the lowest integer N is
    replaced with the formatted value.  If no more references are left
    in the pattern, the formatted value is appended to the string,
    but this usage is not adviced.  The format pattern is scanned for
    argument references only when it is constructed or if #rescan() is
    called; this makes it safe to replace values that might otherwise
    result in further argument references.

    The %-characters in the format pattern cannot be escaped and the
    interpretation of the digits following it cannot be stopped.  Both
    situations are easily handled with an extra replacement pattern:
    StringFormat("%1%2").arg(10).arg('%') produces "10%" and
    StringFormat("%1%2").arg(10).arg("00") produces "1000".

    The absolute value of @a fieldwidth specifies the minimum length
    of the formatted replacement string.  If the formatted value is
    longer than the requested width, it is replaced as such.
    Otherwise it is padded with spaces on right if @a fieldwidth is
    positive, or left if @a fieldwidth is negative.

    Returns a new #StringFormat with the lowest argument reference
    replaced.  */
StringFormat
StringFormat::arg (char val, int fieldwidth /* = 0 */) const
{ return doarg (pad (&val, 1, fieldwidth)); }

/** Replace the next smallest argument reference of the form "%N" in
    the string format with the character @a val padded to a string of
    at least @a fieldwidth wide.

    The argument references are of the form "%N" where the N is an
    arbitrary number.  At each #arg() call the lowest integer N is
    replaced with the formatted value.  If no more references are left
    in the pattern, the formatted value is appended to the string,
    but this usage is not adviced.  The format pattern is scanned for
    argument references only when it is constructed or if #rescan() is
    called; this makes it safe to replace values that might otherwise
    result in further argument references.

    The %-characters in the format pattern cannot be escaped and the
    interpretation of the digits following it cannot be stopped.  Both
    situations are easily handled with an extra replacement pattern:
    StringFormat("%1%2").arg(10).arg('%') produces "10%" and
    StringFormat("%1%2").arg(10).arg("00") produces "1000".

    The absolute value of @a fieldwidth specifies the minimum length
    of the formatted replacement string.  If the formatted value is
    longer than the requested width, it is replaced as such.
    Otherwise it is padded with spaces on right if @a fieldwidth is
    positive, or left if @a fieldwidth is negative.

    Returns a new #StringFormat with the lowest argument reference
    replaced.  */
StringFormat
StringFormat::arg (unsigned char val, int fieldwidth /* = 0 */) const
{ return doarg (pad ((char *) &val, 1, fieldwidth)); }

/** Replace the next smallest argument reference of the form "%N" in
    the string format with the character @a val padded to a string of
    at least @a fieldwidth wide.

    The argument references are of the form "%N" where the N is an
    arbitrary number.  At each #arg() call the lowest integer N is
    replaced with the formatted value.  If no more references are left
    in the pattern, the formatted value is appended to the string,
    but this usage is not adviced.  The format pattern is scanned for
    argument references only when it is constructed or if #rescan() is
    called; this makes it safe to replace values that might otherwise
    result in further argument references.

    The %-characters in the format pattern cannot be escaped and the
    interpretation of the digits following it cannot be stopped.  Both
    situations are easily handled with an extra replacement pattern:
    StringFormat("%1%2").arg(10).arg('%') produces "10%" and
    StringFormat("%1%2").arg(10).arg("00") produces "1000".

    The absolute value of @a fieldwidth specifies the minimum length
    of the formatted replacement string.  If the formatted value is
    longer than the requested width, it is replaced as such.
    Otherwise it is padded with spaces on right if @a fieldwidth is
    positive, or left if @a fieldwidth is negative.

    Returns a new #StringFormat with the lowest argument reference
    replaced.  */
StringFormat
StringFormat::arg (signed char val, int fieldwidth /* = 0 */) const
{ return doarg (pad ((char *) &val, 1, fieldwidth)); }

/** Replace the next smallest argument reference of the form "%N" in
    the string format with the string @a val padded to a string of at
    least @a fieldwidth wide.

    The argument references are of the form "%N" where the N is an
    arbitrary number.  At each #arg() call the lowest integer N is
    replaced with the formatted value.  If no more references are left
    in the pattern, the formatted value is appended to the string,
    but this usage is not adviced.  The format pattern is scanned for
    argument references only when it is constructed or if #rescan() is
    called; this makes it safe to replace values that might otherwise
    result in further argument references.

    The %-characters in the format pattern cannot be escaped and the
    interpretation of the digits following it cannot be stopped.  Both
    situations are easily handled with an extra replacement pattern:
    StringFormat("%1%2").arg(10).arg('%') produces "10%" and
    StringFormat("%1%2").arg(10).arg("00") produces "1000".

    The absolute value of @a fieldwidth specifies the minimum length
    of the formatted replacement string.  If the formatted value is
    longer than the requested width, it is replaced as such.
    Otherwise it is padded with spaces on right if @a fieldwidth is
    positive, or left if @a fieldwidth is negative.

    Returns a new #StringFormat with the lowest argument reference
    replaced.  */
StringFormat
StringFormat::arg (const std::string &val, int fieldwidth /* = 0 */) const
{ return doarg (pad (val.data (), val.size (), fieldwidth)); }

/** Replace the next smallest argument reference of the form "%N" in
    the string format with the conversion of @a val to a string of at
    least @a fieldwidth wide, in floating point conversion format @a
    fmt and with precision @a precision.

    The argument references are of the form "%N" where the N is an
    arbitrary number.  At each #arg() call the lowest integer N is
    replaced with the formatted value.  If no more references are left
    in the pattern, the formatted value is appended to the string,
    but this usage is not adviced.  The format pattern is scanned for
    argument references only when it is constructed or if #rescan() is
    called; this makes it safe to replace values that might otherwise
    result in further argument references.

    The %-characters in the format pattern cannot be escaped and the
    interpretation of the digits following it cannot be stopped.  Both
    situations are easily handled with an extra replacement pattern:
    StringFormat("%1%2").arg(10).arg('%') produces "10%" and
    StringFormat("%1%2").arg(10).arg("00") produces "1000".

    The absolute value of @a fieldwidth specifies the minimum length
    of the formatted replacement string, including a possible leading
    minus sign.  If the formatted value is longer than the requested
    width, it is replaced as such.  Otherwise it is padded with spaces
    on right if @a fieldwidth is positive, or left if @a fieldwidth is
    negative.

    The format @a fmt must be one of 'f', 'e', 'E', 'g', or 'G' and
    specifies the desired floating point representation.  The minimum
    field width (@a fieldwidth) and precision after decimal point (@a
    precision) let the caller specify further conversion parameters.
    The handling of these arguments is identical to the C @c sprintf()
    function -- and similar to how field width is handled in the other
    overloaded #arg() methods.

    Returns a new #StringFormat with the lowest argument reference
    replaced.  */
StringFormat
StringFormat::arg (double		val,
		   int			fieldwidth /* = 0 */,
		   char			fmt /* = 'g' */,
		   int			precision /* = -1 */) const
{ return doarg (format (val, fieldwidth, fmt, precision)); }

} // namespace lat
