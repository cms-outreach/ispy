#ifndef CLASSLIB_SOCKET_PAIR_H
# define CLASSLIB_SOCKET_PAIR_H

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

/** Pair of sockets connected to each other.  */
class SocketPair : public IOChannelPair
{
public:
    // FIXME: split into separate classes per socket type?
    // FIXME:  - family (AF_INET, AF_LOCAL, ...)
    // FIXME:  - type (SOCKET_STREAM, SOCKET_DGRAM, ...)
    // FIXME:  - protocol?

    SocketPair (bool create = true);
    ~SocketPair (void);
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
#endif // CLASSLIB_SOCKET_PAIR_H
