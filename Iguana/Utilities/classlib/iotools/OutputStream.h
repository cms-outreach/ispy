#ifndef CLASSLIB_OUTPUT_STREAM_H
# define CLASSLIB_OUTPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include "classlib/iobase/IOOutput.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** An output data destination.  */
class OutputStream : public virtual IOOutput
{
public:
    virtual ~OutputStream (void);

    virtual void	finish (void);
    virtual void	close (void);

protected:
    OutputStream (void);

private:
    // undefined, no semantics
    OutputStream (const OutputStream &);
    OutputStream &operator= (const OutputStream &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_OUTPUT_STREAM_H
