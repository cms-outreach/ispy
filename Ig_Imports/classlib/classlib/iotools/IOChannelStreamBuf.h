#ifndef CLASSLIB_IO_CHANNEL_STREAM_BUF_H
# define CLASSLIB_IO_CHANNEL_STREAM_BUF_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iotools/CustomStreamBuf.h"
# include "classlib/iotools/ReadBuffer.h"
# include "classlib/iotools/WriteBuffer.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IOChannel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IOChannelStreamBuf : public CustomStreamBuf
{
public:
    IOChannelStreamBuf (IOChannel *data = 0);
    ~IOChannelStreamBuf (void);

    virtual IOChannel *	channel (void);
    virtual void	attach (IOChannel *to);
    virtual bool	attached (void) const;
    virtual void	detach (void);
    virtual bool	close (void);

    virtual size_type	showmanyc (void);
    virtual int_type	overflow (int_type c = CLASSLIB_STREAM_EOF);
    virtual int_type	underflow (void);
    virtual int_type	uflow (void);
    virtual int_type	pbackfail (int_type c = CLASSLIB_STREAM_EOF);
    virtual int_type	sync (void);
    
    virtual size_type	xsputn (const char_type *data, size_type length);
    virtual size_type	xsgetn (char_type *data, size_type length);

private:
    bool		syncwrite (void);
    size_type		writable (void);
    size_type		refill (void);

    IOChannel		*m_data;
    ReadBuffer		m_rbuffer;
    WriteBuffer		m_wbuffer;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_IO_CHANNEL_STREAM_BUF_H
