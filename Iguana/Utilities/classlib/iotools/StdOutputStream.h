#ifndef CLASSLIB_STD_OUTPUT_STREAM_H
# define CLASSLIB_STD_OUTPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iotools/OutputStream.h"
# include <iostream>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class StdOutputStream : public OutputStream
{
public:
    StdOutputStream (std::ostream *output);
    ~StdOutputStream (void);

    using OutputStream::write;
    virtual IOSize	write (const void *from, IOSize n);
    virtual void	finish (void);
    virtual void	close (void);

protected:
    std::ostream	*m_output;	//< Actual output.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_STD_OUTPUT_STREAM_H
