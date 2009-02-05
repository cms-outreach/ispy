#ifndef CLASSLIB_PIPE_H
# define CLASSLIB_PIPE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iobase/IOChannelPair.h"
# include "classlib/iobase/IOChannel.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Pair of file descriptors connected to each other.  */
class Pipe : public IOChannelPair
{
    // FIXME: block SIGPIPE -> EPIPE -> throw?  (write only, read -> eof)
public:
    Pipe (bool create = true);
    // implicit copy constructor
    // implicit destructor
    // implicit assignment operator

    virtual void	open (void);
    virtual IOChannel *	source (void);
    virtual IOChannel *	sink (void);

private:
    IOChannel		m_source;	//< Source channel
    IOChannel		m_sink;		//< Sink channel
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_PIPE_H
