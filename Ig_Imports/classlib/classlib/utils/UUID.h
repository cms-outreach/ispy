#ifndef CLASSLIB_UUID_H
# define CLASSLIB_UUID_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/Error.h"
# include "classlib/utils/iosfwd.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>

# define CONST_UUID(a,b,c,d,e,f,g,h,i,j,k) \
    {0x##a,0x##b,0x##c,0x##d,0x##e,0x##f,0x##g,0x##h,0x##i,0x##j,0x##k}


//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class UUID;
class Time;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

std::ostream &operator<< (std::ostream &out, const UUID &id);

//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class UUID
{
public:
    typedef unsigned char Binary [16];

    UUID (void);
    UUID (const char *text);
    UUID (const std::string &text);
    UUID (Binary raw);
    UUID (const UUID &x);
    UUID &operator= (const UUID &x);

    bool		operator== (const UUID &x) const;
    bool		operator!= (const UUID &x) const;
    bool		operator<  (const UUID &x) const;

    bool		isNull (void) const;
    Time		time (void) const;
    int			type (void) const;
    int			variant (void) const;

    void		format (char *buf) const;

    void		clear (void);

    static UUID		generate (void);
    static UUID		generateRandom (void);
    static UUID		generateTime (void);

private:
    Binary		m_uuid;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_UUID_H
