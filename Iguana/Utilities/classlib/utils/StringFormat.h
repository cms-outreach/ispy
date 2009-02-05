#ifndef CLASSLIB_STRING_FORMAT_H
# define CLASSLIB_STRING_FORMAT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <iosfwd>
# include <string>
# include <utility>
# include <list>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class StringFormat;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

std::ostream &
operator<< (std::ostream &out, const StringFormat &x);

//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class StringFormat
{
public:
    explicit StringFormat (const std::string &pattern);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    const std::string &	value (void) const;
    /**/		operator const std::string & (void) const;

    StringFormat	arg (long		val,
			     int		fieldwidth = 0,
			     int		base = 10) const;
    StringFormat	arg (unsigned long	val,
			     int		fieldwidth = 0,
			     int		base = 10) const;
    StringFormat	arg (int		val,
			     int		fieldwidth = 0,
			     int		base = 10) const;
    StringFormat	arg (unsigned int	val,
			     int		fieldwidth = 0,
			     int		base = 10) const;
    StringFormat	arg (short		val,
			     int		fieldwidth = 0,
			     int		base = 10) const;
    StringFormat	arg (unsigned short	val,
			     int		fieldwidth = 0,
			     int		base = 10) const;
    StringFormat	arg (char		val, int fieldwidth = 0) const;
    StringFormat	arg (unsigned char	val, int fieldwidth = 0) const;
    StringFormat	arg (signed char	val, int fieldwidth = 0) const;
    StringFormat	arg (const std::string &val, int fieldwidth = 0) const;
    StringFormat	arg (double		val,
			     int		fieldwidth = 0,
			     char		format = 'g',
			     int		precision = -1) const;

    StringFormat	rescan (void) const;

private:
    typedef std::list< std::pair<size_t, size_t> > ArgList;

    StringFormat (void);
    StringFormat (const std::string &value, const std::string &replacement,
		  size_t offset, size_t len,
		  ArgList::const_iterator first, ArgList::const_iterator last);

    void		dorescan (void);
    StringFormat	doarg (const std::string &replacement) const;
    static bool		nextarg (const std::string &in,
				 size_t &offset,
				 size_t &len);

    static std::string	pad (const char *buf, size_t len, int fieldwidth);

    static char *	format (char *next, unsigned long val, int base);
    static std::string	format (long val, int fieldwidth, int base);
    static std::string	format (unsigned long val, int fieldwidth, int base);
    static std::string	format (double val, int fieldwidth, char fmt,
				int precision);

    std::string		m_value;
    ArgList		m_args;
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_STRING_FORMAT_H
