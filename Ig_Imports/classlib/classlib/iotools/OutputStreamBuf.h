#ifndef CLASSLIB_OUTPUT_STREAM_BUF_H
# define CLASSLIB_OUTPUT_STREAM_BUF_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iotools/CustomStreamBuf.h"
# include "classlib/iotools/WriteBuffer.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class OutputStream;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class OutputStreamBuf : public CustomStreamBuf
{
public:
    OutputStreamBuf (OutputStream *data = 0);
    ~OutputStreamBuf (void);

    virtual OutputStream *stream (void);
    virtual void	attach (OutputStream *to);
    virtual bool	attached (void) const;
    virtual void	detach (void);
    virtual bool	close (void);

    virtual int_type	overflow (int_type c = CLASSLIB_STREAM_EOF);
    virtual int_type	sync (void);
    virtual size_type	xsputn (const char_type *data, size_type length);

private:
    bool		syncwrite (void);
    size_type		writable (void);

    OutputStream	*m_data;
    WriteBuffer		m_buffer;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_OUTPUT_STREAM_BUF_H
