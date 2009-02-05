#ifndef CLASSLIB_CHECKED_INPUT_STREAM_H
# define CLASSLIB_CHECKED_INPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include "classlib/iotools/FilterInputStream.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Checksum;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class CheckedInputStream : public FilterInputStream
{
public:
    CheckedInputStream (InputStream *input, Checksum *validator);
    ~CheckedInputStream (void);

    using FilterInputStream::read;
    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	skip (IOSize n);
    virtual void	reset (void);
    virtual Checksum *	checksum (void) const;

private:
    Checksum		*m_checksum;	//< Checksum calculator
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_CHECKED_INPUT_STREAM_H
