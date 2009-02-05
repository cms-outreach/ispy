#ifndef CLASSLIB_FILTER_OUTPUT_STREAM_H
# define CLASSLIB_FILTER_OUTPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iotools/OutputStream.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class FilterOutputStream : public OutputStream
{
public:
    FilterOutputStream (OutputStream *output);
    ~FilterOutputStream (void);

    using OutputStream::write;
    virtual IOSize	write (const void *from, IOSize n);
    virtual void	finish (void);
    virtual void	close (void);

protected:
    OutputStream	*m_output;	//< Actual output
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_FILTER_OUTPUT_STREAM_H
