#ifndef CLASSLIB_ZIP_OUTPUT_STREAM_H
# define CLASSLIB_ZIP_OUTPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/ZOutputStream.h"
# include <vector>
# include <string>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class ZipMember;
class MemoryStorage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// Writes a zip archive straight into an output stream.  NB: this
// buffers the member's data until the member is closed so that it can
// write the correct sizes and crc32!  Handle stored and compressed
// files!
class ZipOutputStream : public ZOutputStream
{
public:
    ZipOutputStream (OutputStream *output);
    ~ZipOutputStream (void);

    using ZOutputStream::write;

    virtual void	nextMember (ZipMember *member);
    virtual IOSize	write (const void *from, IOSize n);
    virtual void	closeMember (void);
    virtual void	finish (void);
    virtual void	close (void);

    virtual void	comment (const std::string &text);
    virtual void	method (Method m);
    virtual void	level (unsigned n);

private:
    std::string		m_comment;	//< ZIP archive comment
    Method		m_method;	//< Default compression method
    unsigned		m_level;	//< Default compression level
    IOOffset		m_total;	//< Total bytes written

    std::vector<ZipMember *> m_members;	//< Members already written
    ZipMember		*m_current;	//< Current member we are writing
    MemoryStorage	*m_buffer;	//< Capture buffer
    OutputStream	*m_bufout;	//< Capture buffer or downstream
    IOOffset		m_usize;	//< Uncompressed size of current member
    bool		m_finished;	//< Flag indicating this level is finished
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_ZIP_OUTPUT_STREAM_H
