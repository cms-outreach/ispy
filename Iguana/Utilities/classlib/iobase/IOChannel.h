#ifndef CLASSLIB_IO_CHANNEL_H
# define CLASSLIB_IO_CHANNEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iobase/IOInput.h"
# include "classlib/iobase/IOOutput.h"
# include "classlib/sysapi/IOPlatformData.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IOSelectMethod;
struct IOSelectRequest;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Base class for stream-oriented I/O sources and targets, based
    on the operating system file descriptor. */
class IOChannel : public virtual IOInput, public virtual IOOutput
{
public:
# ifdef CLASSLIB_DEPRECATED // Backwards (1.3.x) compatible settings
    static const int ModeRead		= IOFlags::OpenRead;
    static const int ModeWrite		= IOFlags::OpenRead;
    static const int ModeNonBlock	= IOFlags::OpenNonBlock;
    static const int ModeAppend		= IOFlags::OpenAppend;
    static const int ModeUnbuffered	= IOFlags::OpenUnbuffered;
    static const int ModeCreate		= IOFlags::OpenCreate;
    static const int ModeExclusive	= IOFlags::OpenExclusive;
    static const int ModeTruncate	= IOFlags::OpenTruncate;
    static const int ModeNotCTTY	= IOFlags::OpenNotCTTY;
#endif

    IOChannel (IOFD fd = IOFD_INVALID);
    virtual ~IOChannel (void);
    // implicit copy constructor
    // implicit assignment operator

    using IOInput::read;
    using IOOutput::write;
    
    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	readv (IOBuffer *into, IOSize buffers);

    virtual IOSize	write (const void *from, IOSize n);
    virtual IOSize	writev (const IOBuffer *from, IOSize buffers);

    virtual IOFD	fd (void) const;
    virtual void	fd (IOFD value); // FIXME: dangerous?

    virtual void	close (void);

    virtual void	setBlocking (bool value);
    virtual bool	isBlocking (void) const;

    // FIXME: dup/dup2?
    // FIXME: {set,is}Asynchronous?  (makes sense with posix signals)
    // FIXME: generic fcntl, ioctl support (flags etc.)?
    // FIXME: win32 handle inheritance?
    // FIXME: aio, lio / w32 overlapped i/o? --> IOAsyncManager?

    static IOSize	transfer (IOChannel *from, IOChannel *to,
				  IOOffset &pos, IOSize length);

protected:
    // System-specific implementation for IOSelector
    friend class IOSelector;
    virtual void	select (IOSelectMethod *to, const IOSelectRequest &req);

    // System implementation
    bool		sysclose (IOFD fd, int *error = 0);

private:
    IOFD		m_fd;		/*< System file descriptor. */
    IOPlatformData	m_data;		/*< Extra Windows state: used
					    to hold the event object
					    for non-blocking I/O. */
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_IO_CHANNEL_H
