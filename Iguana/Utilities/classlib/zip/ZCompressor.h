#ifndef CLASSLIB_Z_COMPRESSOR_H
# define CLASSLIB_Z_COMPRESSOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/Compressor.h"
# include "classlib/zip/ZConstants.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Compress output data using the ZLIB algorithm.  */
class ZCompressor : public Compressor, public ZConstants
{
public:
    // nowrap -> no zlib header, checksum
    ZCompressor (unsigned level = DEFAULT_COMPRESSION, bool wrap = false);
    ~ZCompressor (void);

    // Global compressor state
    virtual void	dictionary (void *buffer, IOSize length);
    virtual unsigned	level (void) const;
    virtual void	level (unsigned level);
    virtual void	reset (void);
    virtual void	end (void);

    // Input management
    virtual bool	more (void) const;
    virtual void	input (const void *buffer, IOSize length);
    virtual void	finish (void);
    virtual void	flush (void);
    // virtual IOSize	rest (void) const;

    // Compression
    virtual bool	finished (void) const;
    virtual bool	flushed (void) const;
    virtual IOSize	compress (void *into, IOSize length);

    // Statistics
    virtual unsigned	checksum (void) const;
    virtual IOOffset	in (void) const;
    virtual IOOffset	out (void) const;

private:
    struct Stream;
    Stream		*m_stream;	//< ZLIB state, null if not initialised
    int			m_level;
    int			m_strategy;
    int			m_method;
    int			m_window;
    int			m_mem;
    bool		m_wrap;
    int			m_action;
    int			m_state;

    // no semantics, undefined
    ZCompressor (const ZCompressor &other);
    ZCompressor &operator= (const ZCompressor &other);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_Z_COMPRESSOR_H
