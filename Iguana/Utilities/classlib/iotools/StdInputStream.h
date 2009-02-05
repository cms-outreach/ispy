#ifndef CLASSLIB_STD_INPUT_STREAM_H
# define CLASSLIB_STD_INPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iotools/InputStream.h"
# include <iostream>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class StdInputStream : public InputStream
{
public:
    StdInputStream (std::istream *input);
    ~StdInputStream (void);

    using InputStream::read;
    virtual IOSize	read (void *into, IOSize n);
    virtual void	close (void);

protected:
    std::istream	*m_input;	//< Actual input.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_STD_INPUT_STREAM_H
