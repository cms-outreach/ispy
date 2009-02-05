#ifndef CLASSLIB_CHECKED_OUTPUT_STREAM_H
# define CLASSLIB_CHECKED_OUTPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include "classlib/iotools/FilterOutputStream.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Checksum;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class CheckedOutputStream : public FilterOutputStream
{
public:
    CheckedOutputStream (OutputStream *output, Checksum *validator);
    ~CheckedOutputStream (void);

    using FilterOutputStream::write;
    virtual IOSize	write (const void *from, IOSize n);
    virtual Checksum *	checksum (void) const;

private:
    Checksum		*m_checksum;	//< Checksum calculator
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_CHECKED_OUTPUT_STREAM_H
