#ifndef CLASSLIB_INPUT_STREAM_H
# define CLASSLIB_INPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include "classlib/iobase/IOInput.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** An input data source.  */
class InputStream : public virtual IOInput
{
public:
    virtual ~InputStream (void);

    virtual IOSize	available (void);
    virtual IOSize	skip (IOSize n);
    virtual void	close (void);
    virtual void	reset (void);

protected:
    static const int	SKIP_BUFFER_SIZE = 1024; //< Local buffer size for skip
    InputStream (void);

private:
    // undefined, no semantics
    InputStream (const InputStream &);
    InputStream &operator= (const InputStream &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_INPUT_STREAM_H
